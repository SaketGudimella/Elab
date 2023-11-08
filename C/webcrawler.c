#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <pthread.h>

#define MAX_URL_LENGTH 2048
#define MAX_FILENAME_LENGTH 128
#define MAX_LINKS 100
#define MAX_THREADS 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t threads[MAX_THREADS];
int num_threads = 0;
int crawling = 1; // Added a flag to control crawling

struct CrawlData {
    char* url;
    char* filename;
};

size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    FILE* file = (FILE*)userp;
    size_t totalSize = size * nmemb;
    return fwrite(contents, size, nmemb, file);
}

void get_page(const char* url, const char* file_name);
void extract_hyperlinks(const char* html_file_name, char* extracted_links[], int* num_links);
void* crawler_thread(void* arg);

int main() {
    char url[MAX_URL_LENGTH];
    char input[MAX_URL_LENGTH];

    curl_global_init(CURL_GLOBAL_DEFAULT);

    while (1) {
        printf("Enter URL (or 'quit' to exit): ");
        fflush(stdout);

        if (fgets(input, MAX_URL_LENGTH, stdin) == NULL) {
            // Handle error or exit when there's no input
            break;
        }
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "quit") == 0) {
            crawling = 0; // Set the crawling flag to 0 to stop crawling
            break;
        }

        struct CrawlData* crawl_data = (struct CrawlData*)malloc(sizeof(struct CrawlData));
        crawl_data->url = strdup(input);
        char sanitized_url[MAX_URL_LENGTH];
        int j = 0;
        for (int i = 0; i < strlen(input); i++) {
            if (input[i] == '/' || input[i] == ':') {
                sanitized_url[j++] = '_';
            } else {
                sanitized_url[j++] = input[i];
            }
        }
        sanitized_url[j] = '\0';
        crawl_data->filename = (char*)malloc(MAX_FILENAME_LENGTH);
        snprintf(crawl_data->filename, MAX_FILENAME_LENGTH, "%s.txt", sanitized_url);

        pthread_t thread;
        if (pthread_create(&thread, NULL, crawler_thread, crawl_data) != 0) {
            perror("pthread_create");
            return 1;
        }
        threads[num_threads++] = thread;
    }

    // Wait for the threads to finish 
    for (int i = 0; i < num_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            return 1;
        }
    }

    curl_global_cleanup();

    return 0;
}

void get_page(const char* url, const char* file_name) {
    CURL* easyhandle = curl_easy_init();

    if (easyhandle) {
        curl_easy_setopt(easyhandle, CURLOPT_URL, url);

        char sanitized_filename[MAX_FILENAME_LENGTH];
        int j = 0;

        for (int i = 0; i < strlen(file_name); i++) {
            if (file_name[i] == '/' || file_name[i] == ':') {
                sanitized_filename[j++] = '_';
            } else {
                sanitized_filename[j++] = file_name[i];
            }
        }
        sanitized_filename[j] = '\0';

        FILE* file = fopen(sanitized_filename, "w");

        if (file) {
            // Callback function
            curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, file);
            curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, write_callback);
            
            
            CURLcode res = curl_easy_perform(easyhandle);
            if (res != CURLE_OK) {
                fprintf(stderr, "Failed to download URL: %s (CURL Code: %d)\n", url, res);
            } else {
                fclose(file);
            }
        }
        curl_easy_cleanup(easyhandle);
    } else {
        fprintf(stderr, "Failed to initialize libcurl for URL: %s\n", url);
    }
}

void extract_hyperlinks(const char* html_file_name, char* extracted_links[], int* num_links) {
    FILE* file = fopen(html_file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open the HTML file for reading: %s\n", html_file_name);
        return;
    }

    char line[MAX_URL_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char* href = strstr(line, "href=\"http");
        if (href != NULL) {
            char* link_start = href + 6;
            char* link_end = strchr(link_start, '\"');
            if (link_end != NULL) {
                int link_length = link_end - link_start;

                char* url = (char*)malloc(link_length + 1);
                strncpy(url, link_start, link_length);
                url[link_length] = '\0';

                extracted_links[(*num_links)++] = url;
            }
        } else {
            href = strstr(line, "href=\"https");
            if (href != NULL) {
                char* link_start = href + 6;
                char* link_end = strchr(link_start, '\"');
                if (link_end != NULL) {
                    int link_length = link_end - link_start;

                    char* url = (char*)malloc(link_length + 1);
                    strncpy(url, link_start, link_length);
                    url[link_length] = '\0';

                    extracted_links[(*num_links)++] = url;
                }
            }
        }
    }
    fclose(file);
}

void* crawler_thread(void* arg) {
    struct CrawlData* crawl_data = (struct CrawlData*)arg;

    if (!crawling) { // Check if we should stop crawling
        free(crawl_data->url);
        free(crawl_data->filename);
        free(crawl_data);
        return NULL;
    }

    get_page(crawl_data->url, crawl_data->filename);

    pthread_mutex_lock(&mutex);
    printf("Crawled: %s\n", crawl_data->url);
    pthread_mutex_unlock(&mutex);

    char* extracted_links[MAX_LINKS];
    int num_links = 0;

    extract_hyperlinks(crawl_data->filename, extracted_links, &num_links);

    for (int i = 0; i < num_links; i++) {
        if (!crawling) { // Check if we should stop crawling
            break;
        }
        
        pthread_mutex_lock(&mutex);
        if (num_threads < MAX_THREADS) {
            struct CrawlData* new_crawl_data = (struct CrawlData*)malloc(sizeof(struct CrawlData));
            new_crawl_data->url = extracted_links[i];
            char sanitized_url[MAX_URL_LENGTH];
            int j = 0;
            for (int k = 0; k < strlen(extracted_links[i]); k++) {
                if (extracted_links[i][k] == '/' || extracted_links[i][k] == ':') {
                    sanitized_url[j++] = '_';
                } else {
                    sanitized_url[j++] = extracted_links[i][k];
                }
            }
            sanitized_url[j] = '\0';
            new_crawl_data->filename = (char*)malloc(MAX_FILENAME_LENGTH);
            snprintf(new_crawl_data->filename, MAX_FILENAME_LENGTH, "%s.txt", sanitized_url);
            pthread_t thread;
            if (pthread_create(&thread, NULL, crawler_thread, new_crawl_data) != 0) {
                perror("pthread_create");
            } else {
                threads[num_threads++] = thread;
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    for (int i = 0; i < num_links; i++) {
        free(extracted_links[i]);
    }

    free(crawl_data->url);
    free(crawl_data->filename);
    free(crawl_data);

    return NULL;
}
