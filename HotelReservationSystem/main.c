#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Room {
  int roomnum;          // 객실번호
  char grade[20];       // 객실등급
  int max_people;       // 최대인원

  char customer[20];    // 예약자 이름
  int reserve_people;   // 예약 인원
  int reserved;         // 예약 여부, 0 예약없음, 1 예약됨
};

int loadCards(struct Room* p[]);
void listCards(struct Room* p[], int n);
void makeReserve(struct Room* p[], int n);
void cancelReserve(struct Room* p[], int n);
void findCards(struct Room* p[], int n);
void saveReport(struct Room* p[], int n);
void findRoom(struct Room* p[], int n);
int addCards(struct Room* p[], int n);

int main() {
  struct Room* rooms[100];
  int count;
  int menu;

  count = loadCards(rooms);

  while(1) {
    printf("\n[1]객실목록 [2]예약 [3]예약취소 [4]검색 [5]저장 [6]빈객실찾기 [7]객실추가 [0]종료 >> ");
    scanf("%d", &menu);

    if(menu == 1) {
      listCards(rooms, count);
    }
    else if(menu == 2) {
      makeReserve(rooms, count);
    }
    else if(menu == 3) {
      cancelReserve(rooms, count);
    }
    else if(menu == 4) {
      findCards(rooms, count);
    }
    else if(menu == 5) {
      saveReport(rooms, count);
    }
    else if(menu == 6) {
      findRoom(rooms, count);
    }
    else if(menu == 7) {
      count = addCards(rooms, count);
    }
    else {
      break;
    }
  }

  return 0;
}

int loadCards(struct Room* p[]) {
  FILE* file;
  int n = 0;
  int grade_num;

  file = fopen("rooms.txt", "rt");

  if(file == NULL) {
    printf("rooms.txt 파일을 열 수 없습니다.\n");
    return 0;
  }

  while(1) {
    p[n] = (struct Room*)malloc(sizeof(struct Room));

    if(fscanf(file, "%d %d %d",
              &(p[n]->roomnum),
              &grade_num,
              &(p[n]->max_people)) != 3) {
      free(p[n]);
      break;
    }

    if(grade_num == 1) {
      strcpy(p[n]->grade, "Deluxe");
    }
    else if(grade_num == 2) {
      strcpy(p[n]->grade, "Luxury");
    }
    else if(grade_num == 3) {
      strcpy(p[n]->grade, "Suite");
    }

    p[n]->reserved = 0;
    p[n]->reserve_people = 0;
    strcpy(p[n]->customer, "");

    n++;
  }

  fclose(file);

  printf("%d개의 객실 정보를 읽었습니다.\n", n);

  return n;
}
void listCards(struct Room* p[], int n) {
  printf("\n전체 객실 목록입니다.\n");
  printf("객실번호 / 등급 / 최대인원 / 예약상황\n");

  for(int i = 0; i < n; i++) {
    if(p[i]->reserved == 0) {
      printf("%d호 / %s / %d명 / 예약없음\n",
             p[i]->roomnum,
             p[i]->grade,
             p[i]->max_people);
    }
    else {
      printf("%d호 / %s / %d명 / 예약 : %s %d명\n",p[i]->roomnum,p[i]->grade,p[i]->max_people,p[i]->customer,p[i]->reserve_people);
    }
  }
}

/* 15-5 객실 예약 */
void makeReserve(struct Room* p[], int n) {
  char name[20];
  int people;
  int roomnum;
  int found_i;

  printf("예약하실 고객 이름은? >> ");
  scanf("%s", name);

  printf("예약 인원수를 입력하시오. >> ");
  scanf("%d", &people);

  while(1) {
    found_i = -1;

    printf("예약하실 객실 번호는? >> ");
    scanf("%d", &roomnum);

    for(int i = 0; i < n; i++) {
      if(p[i]->roomnum == roomnum) {
        found_i = i;
        break;
      }
    }

    if(found_i == -1) {
      printf("예약불가(객실번호 오류)\n");
      continue;
    }

    if(p[found_i]->reserved == 1) {
      printf("예약불가(예약된 객실)\n");
      continue;
    }

    if(people > p[found_i]->max_people) {
      printf("예약불가(인원 초과)\n");
      continue;
    }

    strcpy(p[found_i]->customer, name);
    p[found_i]->reserve_people = people;
    p[found_i]->reserved = 1;

    printf("예약완료!\n");
    break;
  }
}

/* 15-6 예약 취소 */
void cancelReserve(struct Room* p[], int n) {
  int roomnum;
  int found_i = -1;
  int answer;

  printf("예약 취소하실 객실 번호는? >> ");
  scanf("%d", &roomnum);

  for(int i = 0; i < n; i++) {
    if(p[i]->roomnum == roomnum) {
      found_i = i;
      break;
    }
  }

  if(found_i == -1 || p[found_i]->reserved == 0) {
    printf("예약된 객실이 아닙니다.\n");
    return;
  }

  printf("객실번호 / 등급 / 예약자 / 예약인원\n");
  printf("%d호 / %s / %s / %d명\n",
         p[found_i]->roomnum,
         p[found_i]->grade,
         p[found_i]->customer,
         p[found_i]->reserve_people);

  printf("위 예약을 취소할까요? (YES 1, NO 0) >> ");
  scanf("%d", &answer);

  if(answer == 1) {
    p[found_i]->reserved = 0;
    p[found_i]->reserve_people = 0;
    strcpy(p[found_i]->customer, "");

    printf("취소했습니다.\n");
  }
}

/* 15-7 객실 검색 */
void findCards(struct Room* p[], int n) {
  char name[20];
  int count = 0;

  while(1) {
    printf("검색하려는 고객 이름을 2글자 이상 입력하세요. >> ");
    scanf("%s", name);

    if(strlen(name) >= 2) {
      break;
    }
  }

  printf("객실번호 / 등급 / 예약자 / 예약인원 / 최대인원\n");

  for(int i = 0; i < n; i++) {
    if(p[i]->reserved == 1 && strstr(p[i]->customer, name) != NULL) {
      printf("%d호 / %s / %s / %d명 / %d명\n",
             p[i]->roomnum,
             p[i]->grade,
             p[i]->customer,
             p[i]->reserve_people,
             p[i]->max_people);
      count++;
    }
  }

  printf("%d명 검색됨\n", count);
}

/* 15-8 객실관리 보고서 저장 */
void saveReport(struct Room* p[], int n) {
  FILE* file;
  char grades[3][20] = {"Deluxe", "Luxury", "Suite"};

  file = fopen("rooms_report.txt", "wt");

  if(file == NULL) {
    printf("rooms_report.txt 파일을 만들 수 없습니다.\n");
    return;
  }

  fprintf(file, "객실 현황 보고서\n\n");

  for(int g = 0; g < 3; g++) {
    int room_count = 0;
    int reserved_room_count = 0;
    int reserved_people_count = 0;
    int remain_people_count = 0;

    for(int i = 0; i < n; i++) {
      if(strcmp(p[i]->grade, grades[g]) == 0) {
        room_count++;

        if(p[i]->reserved == 1) {
          reserved_room_count++;
          reserved_people_count += p[i]->reserve_people;
        }
        else {
          remain_people_count += p[i]->max_people;
        }
      }
    }

    fprintf(file, "객실등급 : %s\n", grades[g]);
    fprintf(file, "1. 객실 수 : 총 %d실 중 %d실 예약 중\n",
            room_count, reserved_room_count);
    fprintf(file, "2. 예약 고객 : 총 %d명\n", reserved_people_count);
    fprintf(file, "3. 남은 최대수용 고객 : 총 %d명\n\n", remain_people_count);
  }

  fprintf(file, "객실 예약 목록\n");
  fprintf(file, "객실번호 / 등급 / 최대인원 / 예약상황\n");

  int total_reserved_people = 0;

  for(int i = 0; i < n; i++) {
    if(p[i]->reserved == 0) {
      fprintf(file, "%d호 / %s / %d명 / 예약없음\n",
              p[i]->roomnum,
              p[i]->grade,
              p[i]->max_people);
    }
    else {
      fprintf(file, "%d호 / %s / %d명 / 예약 : %s %d명\n",
              p[i]->roomnum,
              p[i]->grade,
              p[i]->max_people,
              p[i]->customer,
              p[i]->reserve_people);

      total_reserved_people += p[i]->reserve_people;
    }
  }

  fprintf(file, "예약 인원 합계 : %d명\n", total_reserved_people);

  fclose(file);

  printf("저장되었습니다.\n");
}

/* 15-9 빈 객실 찾기 */
void findRoom(struct Room* p[], int n) {
  int people;
  int grade_num;
  int count = 0;
  char want_grade[20];

  printf("투숙 인원수는? >> ");
  scanf("%d", &people);

  printf("원하시는 객실 등급은? (Deluxe 1, Luxury 2. Suite 3, 무관 0) >> ");
  scanf("%d", &grade_num);

  if(grade_num == 1) {
    strcpy(want_grade, "Deluxe");
  }
  else if(grade_num == 2) {
    strcpy(want_grade, "Luxury");
  }
  else if(grade_num == 3) {
    strcpy(want_grade, "Suite");
  }
  else {
    strcpy(want_grade, "");
  }

  printf("객실번호 / 등급 / 최대인원\n");

  for(int i = 0; i < n; i++) {
    if(p[i]->reserved == 0 && p[i]->max_people >= people) {
      if(grade_num == 0 || strcmp(p[i]->grade, want_grade) == 0) {
        printf("%d호 / %s / %d명\n",
               p[i]->roomnum,
               p[i]->grade,
               p[i]->max_people);
        count++;
      }
    }
  }

  printf("총 %d개의 객실이 예약 가능합니다.\n", count);
}

/* 15-10 객실 추가 */
int addCards(struct Room* p[], int n) {
  int roomnum;
  int grade_num;
  int max_people;
  int exists;
  int connected;

  while(1) {
    exists = 0;
    connected = 0;

    printf("추가할 객실번호는? >> ");
    scanf("%d", &roomnum);

    for(int i = 0; i < n; i++) {
      if(p[i]->roomnum == roomnum) {
        exists = 1;
        break;
      }
    }

    if(exists == 1) {
      printf("이미 등록된 객실번호입니다.\n");
      continue;
    }

    for(int i = 0; i < n; i++) {
      if(p[i]->roomnum == roomnum - 1 || p[i]->roomnum == roomnum + 1) {
        connected = 1;
        break;
      }
    }

    if(connected == 0) {
      printf("연속적인 객실번호가 아닙니다.\n");
      continue;
    }

    break;
  }

  printf("객실등급(Deluxe 1, Luxury 2. Suite 3)과 최대인원을 입력하세요 >> ");
  scanf("%d %d", &grade_num, &max_people);

  p[n] = (struct Room*)malloc(sizeof(struct Room));

  p[n]->roomnum = roomnum;
  p[n]->max_people = max_people;
  p[n]->reserved = 0;
  p[n]->reserve_people = 0;
  strcpy(p[n]->customer, "");

  if(grade_num == 1) {
    strcpy(p[n]->grade, "Deluxe");
  }
  else if(grade_num == 2) {
    strcpy(p[n]->grade, "Luxury");
  }
  else {
    strcpy(p[n]->grade, "Suite");
  }

  printf("객실을 추가하였습니다.\n");

  return n + 1;
}
