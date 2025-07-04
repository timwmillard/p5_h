#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <curl/curl.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define MAX_PATH 1024
#define MAX_COMMAND 2048
#define MAX_TARGETS 64
#define MAX_DEPS 32
#define MAX_FILES 16

typedef struct {
    char name[256];
    char repo[256];
    char files[MAX_FILES][256];
    int file_count;
    char dest[256];
} dependency_t;

typedef struct {
    char name[256];
    char type[64];
    char sources[MAX_FILES][256];
    int source_count;
    char headers[MAX_FILES][256];
    int header_count;
} target_t;

typedef struct {
    char name[256];
    char cc[256];
    char cflags[MAX_FILES][256];
    int cflag_count;
    char libs[MAX_FILES][256];
    int lib_count;
    dependency_t deps[MAX_DEPS];
    int dep_count;
    target_t targets[MAX_TARGETS];
    int target_count;
} build_config_t;

// HTTP response structure for curl
struct http_response {
    char *data;
    size_t size;
};

// Curl callback for writing response data
static size_t write_callback(void *contents, size_t size, size_t nmemb, struct http_response *response) {
    size_t realsize = size * nmemb;
    char *ptr = realloc(response->data, response->size + realsize + 1);
    if (!ptr) {
        printf("Error: Not enough memory (realloc returned NULL)\n");
        return 0;
    }
    
    response->data = ptr;
    memcpy(&(response->data[response->size]), contents, realsize);
    response->size += realsize;
    response->data[response->size] = 0;
    
    return realsize;
}

// Download file from GitHub
int download_file(const char *repo, const char *filename, const char *dest_path) {
    CURL *curl;
    CURLcode res;
    FILE *fp;
    char url[512];
    char full_path[512];
    
    // Create destination directory if it doesn't exist
    struct stat st = {0};
    if (stat(dest_path, &st) == -1) {
        mkdir(dest_path, 0755);
    }
    
    // Build GitHub raw URL
    snprintf(url, sizeof(url), "https://raw.githubusercontent.com/%s/master/%s", repo, filename);
    snprintf(full_path, sizeof(full_path), "%s%s", dest_path, filename);
    
    printf("Downloading %s -> %s\n", url, full_path);
    
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(full_path, "wb");
        if (!fp) {
            printf("Error: Cannot open file %s for writing\n", full_path);
            curl_easy_cleanup(curl);
            return 1;
        }
        
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
        
        res = curl_easy_perform(curl);
        
        fclose(fp);
        curl_easy_cleanup(curl);
        
        if (res != CURLE_OK) {
            printf("Error downloading %s: %s\n", url, curl_easy_strerror(res));
            return 1;
        }
    } else {
        printf("Error: Failed to initialize curl\n");
        return 1;
    }
    
    return 0;
}

// Parse Lua configuration
int parse_lua_config(const char *filename, build_config_t *config) {
    lua_State *L;
    int result = 0;
    
    L = luaL_newstate();
    luaL_openlibs(L);
    
    if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0)) {
        printf("Error loading %s: %s\n", filename, lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }
    
    // Parse project info
    lua_getglobal(L, "project");
    if (lua_istable(L, -1)) {
        lua_pushstring(L, "name");
        lua_gettable(L, -2);
        if (lua_isstring(L, -1)) {
            strncpy(config->name, lua_tostring(L, -1), sizeof(config->name) - 1);
        }
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
    
    // Parse compiler settings
    lua_getglobal(L, "compiler");
    if (lua_istable(L, -1)) {
        lua_pushstring(L, "cc");
        lua_gettable(L, -2);
        if (lua_isstring(L, -1)) {
            strncpy(config->cc, lua_tostring(L, -1), sizeof(config->cc) - 1);
        }
        lua_pop(L, 1);
        
        // Parse cflags
        lua_pushstring(L, "cflags");
        lua_gettable(L, -2);
        if (lua_istable(L, -1)) {
            int i = 1;
            config->cflag_count = 0;
            while (config->cflag_count < MAX_FILES) {
                lua_rawgeti(L, -1, i);
                if (lua_isstring(L, -1)) {
                    strncpy(config->cflags[config->cflag_count], lua_tostring(L, -1), 255);
                    config->cflag_count++;
                    i++;
                } else {
                    lua_pop(L, 1);
                    break;
                }
                lua_pop(L, 1);
            }
        }
        lua_pop(L, 1);
        
        // Parse libs
        lua_pushstring(L, "libs");
        lua_gettable(L, -2);
        if (lua_istable(L, -1)) {
            int i = 1;
            config->lib_count = 0;
            while (config->lib_count < MAX_FILES) {
                lua_rawgeti(L, -1, i);
                if (lua_isstring(L, -1)) {
                    strncpy(config->libs[config->lib_count], lua_tostring(L, -1), 255);
                    config->lib_count++;
                    i++;
                } else {
                    lua_pop(L, 1);
                    break;
                }
                lua_pop(L, 1);
            }
        }
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
    
    // Parse dependencies
    lua_getglobal(L, "dependencies");
    if (lua_istable(L, -1)) {
        int dep_index = 1;
        config->dep_count = 0;
        
        while (config->dep_count < MAX_DEPS) {
            lua_rawgeti(L, -1, dep_index);
            if (lua_istable(L, -1)) {
                dependency_t *dep = &config->deps[config->dep_count];
                
                // Parse name
                lua_pushstring(L, "name");
                lua_gettable(L, -2);
                if (lua_isstring(L, -1)) {
                    strncpy(dep->name, lua_tostring(L, -1), sizeof(dep->name) - 1);
                }
                lua_pop(L, 1);
                
                // Parse repo
                lua_pushstring(L, "repo");
                lua_gettable(L, -2);
                if (lua_isstring(L, -1)) {
                    strncpy(dep->repo, lua_tostring(L, -1), sizeof(dep->repo) - 1);
                }
                lua_pop(L, 1);
                
                // Parse dest
                lua_pushstring(L, "dest");
                lua_gettable(L, -2);
                if (lua_isstring(L, -1)) {
                    strncpy(dep->dest, lua_tostring(L, -1), sizeof(dep->dest) - 1);
                }
                lua_pop(L, 1);
                
                // Parse files
                lua_pushstring(L, "files");
                lua_gettable(L, -2);
                if (lua_istable(L, -1)) {
                    int file_index = 1;
                    dep->file_count = 0;
                    while (dep->file_count < MAX_FILES) {
                        lua_rawgeti(L, -1, file_index);
                        if (lua_isstring(L, -1)) {
                            strncpy(dep->files[dep->file_count], lua_tostring(L, -1), 255);
                            dep->file_count++;
                            file_index++;
                        } else {
                            lua_pop(L, 1);
                            break;
                        }
                        lua_pop(L, 1);
                    }
                }
                lua_pop(L, 1);
                
                config->dep_count++;
                dep_index++;
            } else {
                lua_pop(L, 1);
                break;
            }
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 1);
    
    // Parse targets
    lua_getglobal(L, "targets");
    if (lua_istable(L, -1)) {
        int target_index = 1;
        config->target_count = 0;
        
        while (config->target_count < MAX_TARGETS) {
            lua_rawgeti(L, -1, target_index);
            if (lua_istable(L, -1)) {
                target_t *target = &config->targets[config->target_count];
                
                // Parse name
                lua_pushstring(L, "name");
                lua_gettable(L, -2);
                if (lua_isstring(L, -1)) {
                    strncpy(target->name, lua_tostring(L, -1), sizeof(target->name) - 1);
                }
                lua_pop(L, 1);
                
                // Parse type
                lua_pushstring(L, "type");
                lua_gettable(L, -2);
                if (lua_isstring(L, -1)) {
                    strncpy(target->type, lua_tostring(L, -1), sizeof(target->type) - 1);
                }
                lua_pop(L, 1);
                
                // Parse sources
                lua_pushstring(L, "sources");
                lua_gettable(L, -2);
                if (lua_istable(L, -1)) {
                    int src_index = 1;
                    target->source_count = 0;
                    while (target->source_count < MAX_FILES) {
                        lua_rawgeti(L, -1, src_index);
                        if (lua_isstring(L, -1)) {
                            strncpy(target->sources[target->source_count], lua_tostring(L, -1), 255);
                            target->source_count++;
                            src_index++;
                        } else {
                            lua_pop(L, 1);
                            break;
                        }
                        lua_pop(L, 1);
                    }
                }
                lua_pop(L, 1);
                
                config->target_count++;
                target_index++;
            } else {
                lua_pop(L, 1);
                break;
            }
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 1);
    
    lua_close(L);
    return result;
}

// Install dependencies
int install_dependencies(build_config_t *config) {
    printf("Installing dependencies...\n");
    
    // Initialize curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    for (int i = 0; i < config->dep_count; i++) {
        dependency_t *dep = &config->deps[i];
        printf("Installing %s from %s\n", dep->name, dep->repo);
        
        for (int j = 0; j < dep->file_count; j++) {
            if (download_file(dep->repo, dep->files[j], dep->dest) != 0) {
                printf("Failed to download %s\n", dep->files[j]);
                curl_global_cleanup();
                return 1;
            }
        }
    }
    
    curl_global_cleanup();
    printf("Dependencies installed successfully.\n");
    return 0;
}

// Build targets
int build_targets(build_config_t *config) {
    printf("Building targets...\n");
    
    for (int i = 0; i < config->target_count; i++) {
        target_t *target = &config->targets[i];
        char command[MAX_COMMAND];
        
        // Build command
        snprintf(command, sizeof(command), "%s -o %s", config->cc, target->name);
        
        // Add cflags
        for (int j = 0; j < config->cflag_count; j++) {
            strncat(command, " ", sizeof(command) - strlen(command) - 1);
            strncat(command, config->cflags[j], sizeof(command) - strlen(command) - 1);
        }
        
        // Add libs
        for (int j = 0; j < config->lib_count; j++) {
            strncat(command, " ", sizeof(command) - strlen(command) - 1);
            strncat(command, config->libs[j], sizeof(command) - strlen(command) - 1);
        }
        
        // Add source files
        for (int j = 0; j < target->source_count; j++) {
            strncat(command, " ", sizeof(command) - strlen(command) - 1);
            strncat(command, target->sources[j], sizeof(command) - strlen(command) - 1);
        }
        
        printf("Building %s: %s\n", target->name, command);
        
        if (system(command) != 0) {
            printf("Error building %s\n", target->name);
            return 1;
        }
    }
    
    printf("All targets built successfully.\n");
    return 0;
}

// Print usage
void print_usage(const char *program_name) {
    printf("Usage: %s [command]\n", program_name);
    printf("Commands:\n");
    printf("  install  - Install dependencies\n");
    printf("  build    - Build all targets\n");
    printf("  clean    - Clean build artifacts\n");
    printf("  help     - Show this help message\n");
}

int main(int argc, char *argv[]) {
    build_config_t config = {0};
    
    // Parse build.lua
    if (parse_lua_config("build.lua", &config) != 0) {
        printf("Error: Failed to parse build.lua\n");
        return 1;
    }
    
    printf("Project: %s\n", config.name);
    
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    const char *command = argv[1];
    
    if (strcmp(command, "install") == 0) {
        return install_dependencies(&config);
    } else if (strcmp(command, "build") == 0) {
        return build_targets(&config);
    } else if (strcmp(command, "clean") == 0) {
        // Clean build artifacts
        for (int i = 0; i < config.target_count; i++) {
            char rm_command[256];
            snprintf(rm_command, sizeof(rm_command), "rm -f %s", config.targets[i].name);
            system(rm_command);
        }
        printf("Build artifacts cleaned.\n");
        return 0;
    } else if (strcmp(command, "help") == 0) {
        print_usage(argv[0]);
        return 0;
    } else {
        printf("Unknown command: %s\n", command);
        print_usage(argv[0]);
        return 1;
    }
    
    return 0;
}