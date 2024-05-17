#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#define LEN_MIN 15 //기차길이
#define LEN_MAX 50
#define PROB_MIN 10 //확률
#define PROB_MAX 90
#define STM_MIN 0 //마동석 체력
#define STM_MAX 5
#define AGGRO_MIN 0 //어그로 범위
#define AGGRO_MAX 5

// 마동석 이동 방향
#define MOVE_LEFT 1
#define MOVE_STAY 0

// 좀비의 공격 대상
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

// 마동석 행동
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

// 전역변수
int train_len; //기차 길이
int percent; //확률	
int stamina;
int cPosition; // 'C,Z,M'의 초기 위치
int zPosition;
int mPosition;
bool cMoved = false;
bool zMoved = false;
bool cannotMoved = false;
int zombieCount = 0;
// 어그로 변수
int citizen_aggro = 1;
int madongseok_aggro = 1;
// 함수 선언
void printIntro() {
    printf("======================================\n");
    printf("=              부산헹                =\n");
    printf("======================================\n");
    Sleep(2000);
}

void trainSetting() {
	printf("train length(%d %d)>> ", LEN_MIN, LEN_MAX);
	scanf_s("%d", &train_len);
	printf("percentile probability 'p'(%d %d)>> ", PROB_MIN, PROB_MAX);
	scanf_s("%d", &percent);
	printf("\n");
    if (train_len < 15 || train_len > 50 || percent < 10 || percent > 90) {
        printf("잘못된 입력으로 프로그램을 종료합니다.\n");
        return 0;
    }
    // 'C,Z'의 초기 위치
    int cPosition = train_len - 6;
    int zPosition = train_len - 3;
}

void percentage() {
    int randomNum = rand() % 100 + 1; // 1에서 100까지의 난수 생성
    // 입력 받은 확률로 cPosition-- or zPosition--
    zombieCount += 1; // 게속 1식 올라가면서 zombieCount가 홀수일 때만 Z가 이동(zPosition--)
    if (zombieCount % 2 == 1) {
        if (randomNum <= percent) {
            zPosition--;
            zMoved = true;
        }
        else zMoved = false;
    }
    else { //zombieCount가 짝수이면 Z는 움직이지 않고 cannot move
        zMoved = false;
        cannotMoved = true;
    }
    if (randomNum <= 100 - percent) { //100-p 확률
        cPosition--;
        cMoved = true;
    }
    else cMoved = false;
}

void train_situation() {
    // 열차 상단 및 하단 경계 출력
    printf("\n\n");
    for (int i = 0; i < train_len; i++) {
        printf("#");
    }
    printf("\n#");

    // 열차 내부
    for (int i = 1; i < train_len - 1; i++) { //열차길이 - 1 만큼 반복
        if (i == cPosition) { //초기 cPosition은 train_len - 6, cPosition이 감소하면 그 위치에 문자출력 
            printf("C");
        }
        else if (i == zPosition) {
            printf("Z");
        }
        else if (i == mPosition) { //열차 끝에서 2번째에 M출력
            printf("M");
            //i += 1;
        }
        else { //위 조건 해당안되면 공백 출력
            printf(" ");
        }
    }
    printf("#\n");
    // 열차 하단 경계 출력
    for (int i = 0; i < train_len; i++) {
        printf("#");
    }
    printf("\n");
}

void citizen_movement() {
    printf("\n\n");
    if (cMoved) {
        printf("citizen: %d -> %d\n", cPosition + 1, cPosition);
    }
    else {
        printf("citizen: stay %d\n", cPosition);
    }
}

void zombie_movement() {
    if (zMoved) {
        printf("zombie: %d -> %d\n", zPosition + 1, zPosition);
    }
    else if (cannotMoved) {
        printf("zombie: stay %d (cannot move)\n", zPosition);
    }
    else {
        printf("zombie: stay %d\n", zPosition);
    }
}
bool printStatus() {
    // C가 끝에 도착하면 탈출 성공 출력 후 프로그램 종료
    if (cPosition == 1) { // 끝이 1
        printf("\nSUCCESS!\n");
        printf("citizen(s) escaped to the next train\n");
        return true;
    }
    // Z가 C바로 옆에 도착하면 구출 실패 출력 후 프로그램 종료
    if (cPosition == zPosition - 1) {
        printf("\nGAME OVER\n");
        printf("citizen(s) has(have) been attacked by a zombie\n");
        return true;
    }
    return false;
}

int main(void) {
    srand((unsigned int)time(NULL));
    printIntro();
    trainSetting();
    //firstTrain();
    train_situation();

    while (1) {
        cMoved = false;
        zMoved = false;
        cannotMoved = false;

        percentage();
        train_situation();
        citizen_movement(); zombie_movement();

        if (printStatus()) {
            break;
        }
    }
}