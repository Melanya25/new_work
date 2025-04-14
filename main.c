#include "mongoose.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define DIR_TEMPLATES "./templates/"
#define DIR_CSS "./css/"

#define PATH_INDEX_HTML DIR_TEMPLATES "index.html"
#define PATH_RESULT_HTML DIR_TEMPLATES "result.html"
#define PATH_ERROR_HTML DIR_TEMPLATES "error.html"
#define PATH_CSS_STYLES DIR_CSS "styles.css"

#define CONTENT_TYPE_HTML "Content-Type: text/html; charset=utf-8\n"
#define CONTENT_TYPE_CSS "Content-Type: text/css; charset=utf-8\n"

char *read_file(const char *filename) {
    FILE *f = fopen(filename, "rb");
    char *res = NULL;
    if (f) {
        fseek(f, 0, SEEK_END);
        size_t size = ftell(f);
        rewind(f);
        res = (char *)malloc(size + 1);
        if (res) {
            fread(res, 1, size, f);
            res[size] = '\0';
        }
        fclose(f);
    }
    return res;
}

static const char *get_grade(double average) {
    if (average >= 4.5) return "Отлично";
    if (average >= 3.5) return "Хорошо";
    if (average >= 2.5) return "Удовлетворительно";
    return "Неудовлетворительно";
}

static void send_response(struct mg_connection *c, const char *filename, const char *content_type) {
    char *content = read_file(filename);
    if (content) {
        mg_http_reply(c, 200, content_type, "%s", content);
        free(content);
    } else {
        mg_http_reply(c, 500, "", "Internal Server Error");
    }
}

static void handle_form_submission(struct mg_connection *c, struct mg_http_message *hm) {
    double grades[5];
    int valid = 1;
    
    // Извлекаем оценки из формы
    for (int i = 0; i < 5; i++) {
        char buf[10];
        mg_http_get_var(&hm->body, mg_strprintf("grade%d", i+1), buf, sizeof(buf));
        grades[i] = atof(buf);
        if (grades[i] < 1 || grades[i] > 5) valid = 0;
    }
    
    if (!valid) {
        send_response(c, PATH_ERROR_HTML, CONTENT_TYPE_HTML);
        return;
    }
    
    // Вычисляем средний балл
    double sum = 0;
    for (int i = 0; i < 5; i++) sum += grades[i];
    double average = sum / 5;
    
    // Формируем HTML с результатами
    char *template = read_file(PATH_RESULT_HTML);
    if (!template) {
        mg_http_reply(c, 500, "", "Internal Server Error");
        return;
    }
    
    char response[1024];
    snprintf(response, sizeof(response), template, average, get_grade(average));
    mg_http_reply(c, 200, CONTENT_TYPE_HTML, "%s", response);
    free(template);
}

static void fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        
        if (mg_http_match_uri(hm, "/styles.css")) {
            send_response(c, PATH_CSS_STYLES, CONTENT_TYPE_CSS);
        } else if (mg_http_match_uri(hm, "/calculate") && mg_vcasecmp(&hm->method, "POST") == 0) {
            handle_form_submission(c, hm);
        } else {
            send_response(c, PATH_INDEX_HTML, CONTENT_TYPE_HTML);
        }
    }
}

int main(void) {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, "http://localhost:8080", fn, NULL);
    
    printf("Server running on http://localhost:8080\n");
    
    while (true) mg_mgr_poll(&mgr, 1000);
    
    mg_mgr_free(&mgr);
    return 0;
}