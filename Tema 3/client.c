#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;

    char command[101];
    scanf("%s", command);

    char cookie[101];
    cookie[0]= NULL;
    char token_library[501];
    token_library[0] = NULL;

    int already_logged_in = 0;

    while(command){

        //inregistratea unui cont - register
        if(strcmp(command, "register") ==  0){
            sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);

            char username[101];
            char password[101];
            JSON_Value *root_value = json_value_init_object();
            JSON_Object *root_object = json_value_get_object(root_value);
            char *serialized_string = NULL;
            JSON_Value *json_response = NULL;

            printf("username=");
            scanf("%s", username);
            printf("password=");
            scanf("%s", password);

            json_object_set_string(root_object, "username", username);
            json_object_set_string(root_object, "password", password);
            serialized_string = json_serialize_to_string_pretty(root_value);

            char **form_data = calloc(2, sizeof(char *));
            for (int i = 0; i < 1; i++){
                form_data[i] = calloc(LINELEN, sizeof(char));
            }
            strcpy(form_data[0], serialized_string);

            message = compute_post_request("34.241.4.235", "/api/v1/tema/auth/register", "application/json", form_data, 1, NULL, 0, NULL);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            char *ok = strstr(response, "HTTP/1.1 201");

            if(ok){
                printf("Registration was executed successfully\n");
            } else {
                json_response = json_parse_string(strstr(response, "{"));
                JSON_Object *obj = json_value_get_object(json_response);
                printf("Error:%s\n", json_object_get_string(obj, "error"));
                
            }
            
            free(form_data);
            json_free_serialized_string(serialized_string);
            json_value_free(json_response);
            json_value_free(root_value);
        }

        //autentificare - login
        if(strcmp(command, "login") ==  0){

            sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);

            char username[101];
            char password[101];
            JSON_Value *root_value = json_value_init_object();
            JSON_Object *root_object = json_value_get_object(root_value);
            char *serialized_string = NULL;
            JSON_Value *json_response = NULL;

            printf("username=");
            scanf("%s", username);
            printf("password=");
            scanf("%s", password);

            json_object_set_string(root_object, "username", username);
            json_object_set_string(root_object, "password", password);
            serialized_string = json_serialize_to_string_pretty(root_value);

            char **form_data = calloc(2, sizeof(char *));
            for (int i = 0; i < 1; i++){
                form_data[i] = calloc(LINELEN, sizeof(char));
            }
            strcpy(form_data[0], serialized_string);

            message = compute_post_request("34.241.4.235", "/api/v1/tema/auth/login", "application/json", form_data, 1, NULL, 0, NULL);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            char *ok = strstr(response, "HTTP/1.1 200");

            if(ok){
                printf("Login was executed successfully\n");
                already_logged_in = 1;
                char *ok = strstr(response, "connect.sid");
                if(ok == NULL){
                    continue;
                }
                char *cookie_login = strtok(ok, ";");
                strcpy(cookie, cookie_login);
            } else{
                json_response = json_parse_string(strstr(response, "{"));
                JSON_Object *obj = json_value_get_object(json_response);
                printf("Error:%s\n", json_object_get_string(obj, "error"));
            }
            
            free(form_data);
            json_free_serialized_string(serialized_string);
            json_value_free(json_response);
            json_value_free(root_value);

        }

        //cerere acces in biblioteca - enter_library
        if(strcmp(command, "enter_library") ==  0){

            sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);

            char **cookies = malloc(101 * sizeof(char *));
            cookies[0] = malloc(101*sizeof(char));
            strcpy(cookies[0], cookie);

            if(cookie[0] == NULL){
                message = compute_get_request("34.241.4.235", "/api/v1/tema/library/access", NULL, NULL, 0, NULL);
            } else {
                message = compute_get_request("34.241.4.235", "/api/v1/tema/library/access", NULL, cookies, 1, NULL);
            }
            
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            char *ok = strstr(response, "HTTP/1.1 200");

            if(ok){
                char *aux = strstr(response, "\"}");
                *(aux) = '\0';
                char *token = strstr(response, "{\"token\":");
                token = token + 10;
                strcpy(token_library, token);
                token_library[strlen(token)] = NULL;
                printf("Entering the library was executed succesfully\n");

            } else {
                JSON_Value *json_response = NULL;
                json_response = json_parse_string(strstr(response, "{"));

                JSON_Object *obj = json_value_get_object(json_response);
                printf("Error:%s\n", json_object_get_string(obj, "error"));

                json_value_free(json_response);
            }

            free(cookies);
        }

        //vizualizarea informatiilor sumare despre toate cartile - get_books
        if(strcmp(command, "get_books") == 0){

            sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);

            JSON_Value *json_response = json_value_init_object();

            char **cookies = malloc(101 * sizeof(char *));
            cookies[0] = malloc(101*sizeof(char));                
            strcpy(cookies[0], cookie);

            if(token_library[0] == NULL){
                 message = compute_get_request("34.241.4.235", "/api/v1/tema/library/books", NULL, cookies, 1, NULL);
            } else{
                message = compute_get_request("34.241.4.235", "/api/v1/tema/library/books", NULL, cookies, 1, token_library);
            }

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            char *ok = strstr(response, "HTTP/1.1 200");

            if(ok){
                json_response = json_parse_string(strstr(response, "["));
                char *serialized_string = json_serialize_to_string_pretty(json_response);
                printf("%s\n", serialized_string);
                json_free_serialized_string(serialized_string);
            }else{
                json_response = json_parse_string(strstr(response, "{"));

                JSON_Object *obj = json_value_get_object(json_response);
                printf("Error:%s\n", json_object_get_string(obj, "error"));
            }
            free(cookies);
            json_value_free(json_response);

        }

        //vizualizarea detaliilor despre o carte - get_book
        if(strcmp(command, "get_book") == 0){

            sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);

            char **cookies = malloc(101 * sizeof(char *));
            cookies[0] = malloc(101*sizeof(char));                
            strcpy(cookies[0], cookie);

            char book[201], id[101];
            strcpy(book,"/api/v1/tema/library/books/");

            printf("id=");
            scanf("%s", id);
            strcat(book, id);


            if(token_library[0] == NULL){
                 message = compute_get_request("34.241.4.235", book, NULL, cookies, 1, NULL);
            } else{
                message = compute_get_request("34.241.4.235", book, NULL, cookies, 1, token_library);
            }
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            char *ok = strstr(response, "HTTP/1.1 200");

            if(ok){
                JSON_Value *json_response = json_value_init_object();
                json_response = json_parse_string(strstr(response, "{"));
                char *serialized_string = json_serialize_to_string_pretty(json_response);
                printf("%s\n", serialized_string);

                json_free_serialized_string(serialized_string);
                json_value_free(json_response);
            }else{

                JSON_Value *json_response = NULL;
                json_response = json_parse_string(strstr(response, "{"));

                JSON_Object *obj = json_value_get_object(json_response);
                printf("Error:%s\n", json_object_get_string(obj, "error"));

                json_value_free(json_response);
            }

            free(cookies);
        }
        
        //adaugarea unei carti - add_book
        if(strcmp(command, "add_book") == 0){
            sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);

            JSON_Value *root_value = json_value_init_object();
            JSON_Object *root_object = json_value_get_object(root_value);
            char *serialized_string = NULL;
            JSON_Value *json_response = NULL;

            char **cookies = malloc(101 * sizeof(char *));
            cookies[0] = malloc(101*sizeof(char));                
            strcpy(cookies[0], cookie);

            if(token_library[0] == NULL){

                message = compute_post_request("34.241.4.235", "/api/v1/tema/library/books", "application/json", NULL, 0, cookies, 1, NULL);

            } else { 
                char title[101], author[101], genre[101], publisher[101];
                char page_count[20];

                printf("title=");
                scanf("%s", title);
                printf("author=");
                scanf("%s", author);
                printf("genre=");
                scanf("%s", genre);
                printf("publisher=");
                scanf("%s", publisher);
                printf("page_count=");
                scanf("%s", page_count);

                json_object_set_string(root_object, "title", title);
                json_object_set_string(root_object, "author", author);
                json_object_set_string(root_object, "genre", genre);
                json_object_set_number(root_object, "page_count", atoi(page_count));
                json_object_set_string(root_object, "publisher", publisher);
                serialized_string = json_serialize_to_string_pretty(root_value);

                char **form_data = calloc(2, sizeof(char *));
                for (int i = 0; i < 1; i++){
                    form_data[i] = calloc(LINELEN, sizeof(char));
                }
                strcpy(form_data[0], serialized_string);

                message = compute_post_request("34.241.4.235", "/api/v1/tema/library/books", "application/json", form_data, 1, cookies, 1, token_library);
                free(form_data);
            }

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            char *ok = strstr(response, "HTTP/1.1 200");

            if(ok){
                printf("Book was added successfully\n");
            } else {
                json_response = json_parse_string(strstr(response, "{"));
                JSON_Object *obj = json_value_get_object(json_response);
                printf("Error:%s\n", json_object_get_string(obj, "error"));
                
            }
            
            free(cookies);
            json_free_serialized_string(serialized_string);
            json_value_free(json_response);
            json_value_free(root_value);

        }

        //stergerea unei carti - delete_book
        if(strcmp(command, "delete_book") == 0){

            sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);

            char **cookies = malloc(101 * sizeof(char *));
            cookies[0] = malloc(101*sizeof(char));                
            strcpy(cookies[0], cookie);

            char book[201], id[101];
            strcpy(book,"/api/v1/tema/library/books/");

            printf("id=");
            scanf("%s", id);
            strcat(book, id);


            if(token_library[0] == NULL){
                 message = compute_delete_request("34.241.4.235", book, NULL, cookies, 1, NULL);
            } else{
                message = compute_delete_request("34.241.4.235", book, NULL, cookies, 1, token_library);
            }
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            char *ok = strstr(response, "HTTP/1.1 200");

            if(ok){
                JSON_Value *json_response = json_value_init_object();
                json_response = json_parse_string(strstr(response, "{"));
                printf("Book deleted succesfully!\n");
            }else{
                JSON_Value *json_response = NULL;
                json_response = json_parse_string(strstr(response, "{"));

                JSON_Object *obj = json_value_get_object(json_response);
                printf("Error:%s\n", json_object_get_string(obj, "error"));
            }
        }

        //logout - logout     
        if(strcmp(command, "logout") == 0){

            sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);

            JSON_Value *json_response = NULL;

            char **cookies = malloc(101 * sizeof(char *));
            cookies[0] = malloc(101*sizeof(char));
            strcpy(cookies[0], cookie);

            if(cookie[0] == NULL){
                message = compute_get_request("34.241.4.235", "/api/v1/tema/library/access", NULL, NULL, 0, token_library);
            } else {
                message = compute_get_request("34.241.4.235", "/api/v1/tema/library/access", NULL, cookies, 1, NULL);
            }

            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            json_response = json_parse_string(strstr(response, "{"));

            char *ok = strstr(response, "HTTP/1.1 200");

            if(ok){
                printf("Logout was executed successfully\n");
                already_logged_in = 0;
                memset(cookies, 0, 101);
                memset(token_library, 0, 501);
                
            } else{
                JSON_Object *obj = json_value_get_object(json_response);
                printf("Error:%s\n", json_object_get_string(obj, "error"));
            }

        }

        if(strcmp(command, "exit") ==  0){
            sockfd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);  

            return 0;
        }

        scanf("%s", command);
    }

    free(message);
    free(response);
    free(command);
    free(cookie);
    free(token_library);

    return 0;
}
