#include "mongoose.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Обработчик HTTP-запросов
void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *)ev_data;
    
    if (mg_http_match_uri(hm, "/calculate")) {
      // Обработка данных формы
      char grade1[10], grade2[10], grade3[10], grade4[10], grade5[10];
      mg_http_get_var(&hm->body, "grade1", grade1, sizeof(grade1));
      mg_http_get_var(&hm->body, "grade2", grade2, sizeof(grade2));
      mg_http_get_var(&hm->body, "grade3", grade3, sizeof(grade3));
      mg_http_get_var(&hm->body, "grade4", grade4, sizeof(grade4));
      mg_http_get_var(&hm->body, "grade5", grade5, sizeof(grade5));
      
      // Расчет среднего балла
      double avg = (atof(grade1) + atof(grade2) + atof(grade3) + 
                   atof(grade4) + atof(grade5)) / 5;
      
      // Отправка результата
      mg_http_reply(c, 200, "Content-Type: text/html\n",
        "<html><body>"
        "<h1>Результат</h1>"
        "<p>Средний балл: %.2f</p>"
        "</body></html>", avg);
    }
    else {
      // Отправка формы по умолчанию
      mg_http_reply(c, 200, "Content-Type: text/html\n",
        "<html><body>"
        "<form method='POST' action='/calculate'>"
        "<input type='number' name='grade1' min='1' max='5' step='0.1' required>"
        "<input type='number' name='grade2' min='1' max='5' step='0.1' required>"
        "<input type='number' name='grade3' min='1' max='5' step='0.1' required>"
        "<input type='number' name='grade4' min='1' max='5' step='0.1' required>"
        "<input type='number' name='grade5' min='1' max='5' step='0.1' required>"
        "<button type='submit'>Рассчитать</button>"
        "</form>"
        "</body></html>");
    }
  }
}

int main() {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, NULL);
  
  printf("Сервер запущен на http://0.0.0.0:8000\n");
  
  while (true) mg_mgr_poll(&mgr, 1000);
  mg_mgr_free(&mgr);
  return 0;
}
