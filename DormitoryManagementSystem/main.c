#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Record{          // 생활관생 관리 레코드
  char name[20];        // 성명              
  char fm;              // 성별 ('F' 여성, 'M' 남성)              
  int entry;            // 입학년도
  int room;             // 방 호실
  int point;            // 벌점
};

int loadMembers(struct Record* p[]);
void listMembers(struct Record* p[], int n);
void editRoom(struct Record* p[], int n);
void makePenalty(struct Record* p[], int n);
void findMembers(struct Record* p[], int n);
void saveReport(struct Record* p[], int n);

int main(){
  struct Record* members[100];
  int count;
  int menu;

  count = loadMembers(members);

  while(1)
  {
    printf("\n[1]목록 [2]호실변경 [3]벌점부여 [4]검색 [5]보고서 저장 [0]종료 >> ");
    scanf("%d",&menu);

    if(menu==1){
      listMembers(members, count);
    }
    else if(menu==2){
      editRoom(members, count);
    }
    else if(menu==3){
      makePenalty(members, count);
    }
    else if(menu==4){
      findMembers(members, count);
    }
    else if(menu==5){
      saveReport(members, count);
    }
    else {
      break;
    }
  }

  return 0;
}

int loadMembers(struct Record* p[]){
  FILE *file;
  int n = 0;

  file = fopen("members.txt","rt");

  while(1){
    p[n] = (struct Record*)malloc(sizeof(struct Record));

    if(fscanf(file,"%s %c %d %d %d",
              p[n]->name,
              &(p[n]->fm),
              &(p[n]->entry),
              &(p[n]->room),
              &(p[n]->point)) != 5) {
      free(p[n]);
      break;
    }

    n++;
  }

  fclose(file);
  printf("%d명이 저장된 파일을 읽었습니다.\n", n);
  return n;
}

void listMembers(struct Record * p[], int n){
  int i;

  printf("\n전체목록입니다.\n");
  printf("NO 이름 성별 학번 호실 벌점\n");

  for(i=0; i<n; i++){
    printf("%d %s [%c] %d %d %d\n",
           i+1,
           p[i]->name,
           p[i]->fm,
           p[i]->entry,
           p[i]->room,
           p[i]->point);
  }
}

// 15-1 호실 변경
void editRoom(struct Record* p[], int n){
  char name[20];
  struct Record* rp;
  int i, found = 0, newroom;

  printf("호실 변경하려는 학생 이름은? >> ");
  scanf("%s", name);

  for(i=0; i<n; i++){
    if(strcmp(p[i]->name, name) == 0){
      found = 1;
      rp = p[i];
      break;
    }
  }

  if(found == 0){
    printf("없는 학생입니다.\n");
    return;
  }

  printf("이름 성별 학번 호실 벌점\n");
  printf("%s [%c] %d %d %d\n",
         rp->name, rp->fm, rp->entry, rp->room, rp->point);

  while(1){
    int room_count = 0;

    printf("새로운 호실을 입력하시오. >> ");
    scanf("%d", &newroom);

    // 이전 호실과 같으면 그대로 수정완료 처리
    if(newroom == rp->room){
      rp->room = newroom;
      printf("수정완료!\n");
      break;
    }

    // 호실 범위 검사
    if(!((newroom >= 101 && newroom <= 115) ||
         (newroom >= 201 && newroom <= 215))){
      printf("배정불가(호실 오류)\n");
      continue;
    }

    // 성별에 따른 층 검사
    if(rp->fm == 'F' && !(newroom >= 101 && newroom <= 115)){
      printf("배정불가(층 오류)\n");
      continue;
    }

    if(rp->fm == 'M' && !(newroom >= 201 && newroom <= 215)){
      printf("배정불가(층 오류)\n");
      continue;
    }

    // 인원 초과 검사
    for(i=0; i<n; i++){
      if(p[i]->room == newroom){
        room_count++;
      }
    }

    if(room_count >= 2){
      printf("배정불가(인원초과)\n");
      continue;
    }

    rp->room = newroom;
    printf("수정완료!\n");
    break;
  }
}

// 15-2 벌점 부여
void makePenalty(struct Record* p[], int n){
  char name[20];
  int i, found = 0;
  int answer;
  struct Record* rp;

  printf("벌점 부여할 학생 이름은? >> ");
  scanf("%s", name);

  for(i=0; i<n; i++){
    if(strcmp(p[i]->name, name) == 0){
      found = 1;
      rp = p[i];
      break;
    }
  }

  if(found == 0){
    printf("없는 학생입니다.\n");
    return;
  }

  printf("이름 성별 학번 호실 벌점\n");
  printf("%s [%c] %d %d %d\n",
         rp->name, rp->fm, rp->entry, rp->room, rp->point);

  printf("위 학생에게 벌점을 부여할까요? (YES 1, NO 0) >> ");
  scanf("%d", &answer);

  if(answer == 1){
    rp->point++;
    printf("%s 학생의 벌점은 %d점입니다.\n", rp->name, rp->point);
  }
}

// 15-3 검색
void findMembers(struct Record * p[], int n){
  int i, count = 0;
  char name[20];

  while(1){
    printf("검색하려는 학생 이름을 2글자 이상 입력하세요. >> ");
    scanf("%s", name);

    if(strlen(name) > 1) break;
  }

  printf("NO 이름 성별 학번 호실 벌점\n");

  for(i=0; i<n; i++){
    if(strstr(p[i]->name, name) != NULL){
      printf("%d %s [%c] %d %d %d\n",
             i+1,
             p[i]->name,
             p[i]->fm,
             p[i]->entry,
             p[i]->room,
             p[i]->point);
      count++;
    }
  }

  printf("%d명 검색됨\n", count);
}

// 15-4 보고서 저장
void saveReport(struct Record* p[], int n){
  FILE* file1;
  int male = 0, female = 0;
  int entry_count[4] = {0};
  int high_count = 0;

  file1 = fopen("report.txt","wt");

  for(int i=0; i<n; i++){
    if(p[i]->fm == 'M') male++;
    else if(p[i]->fm == 'F') female++;

    if(p[i]->entry >= 2015 && p[i]->entry <= 2018){
      entry_count[p[i]->entry - 2015]++;
    }

    if(p[i]->point >= 7){
      high_count++;
    }
  }

  fprintf(file1, "다윗관 현황 보고서\n");
  fprintf(file1, "1. 거주학생 수 : %d명 (남 %d명, 여 %d명)\n\n", n, male, female);

  fprintf(file1, "2. 호실 현황\n");

  fprintf(file1, " 1) 공실 : ");
  for(int floor=1; floor<=2; floor++){
    for(int room=1; room<=15; room++){
      int roomnum = floor * 100 + room;
      int count = 0;

      for(int i=0; i<n; i++){
        if(p[i]->room == roomnum) count++;
      }

      if(count == 0){
        fprintf(file1, "%d ", roomnum);
      }
    }
  }
  fprintf(file1, "\n");

  fprintf(file1, " 2) 1인실 : ");
  for(int floor=1; floor<=2; floor++){
    for(int room=1; room<=15; room++){
      int roomnum = floor * 100 + room;
      int count = 0;

      for(int i=0; i<n; i++){
        if(p[i]->room == roomnum) count++;
      }

      if(count == 1){
        fprintf(file1, "%d ", roomnum);
      }
    }
  }
  fprintf(file1, "\n");

  fprintf(file1, " 3) 2인실 : ");
  for(int floor=1; floor<=2; floor++){
    for(int room=1; room<=15; room++){
      int roomnum = floor * 100 + room;
      int count = 0;

      for(int i=0; i<n; i++){
        if(p[i]->room == roomnum) count++;
      }

      if(count == 2){
        fprintf(file1, "%d ", roomnum);
      }
    }
  }
  fprintf(file1, "\n\n");

  fprintf(file1, "3. 학번별 현황\n");
  for(int i=0; i<4; i++){
    fprintf(file1, " %d) %d학번 : %d명\n",
            i+1, 2015+i, entry_count[i]);
  }
  fprintf(file1, "\n");

  fprintf(file1, "4. 고벌점자(7점 이상) : %d명\n", high_count);
  for(int i=0; i<n; i++){
    if(p[i]->point >= 7){
      fprintf(file1, "%s(%d) ", p[i]->name, p[i]->point);
    }
  }
  fprintf(file1, "\n\n");

  fprintf(file1, "5. 관생 목록\n");
  fprintf(file1, "NO\t이름\t성별\t학번\t호실\t벌점\n");

  for(int i=0; i<n; i++){
    fprintf(file1, "%d\t%s\t[%c]\t%d\t%d\t%d\n",
            i+1,
            p[i]->name,
            p[i]->fm,
            p[i]->entry,
            p[i]->room,
            p[i]->point);
  }

  fclose(file1);
}
