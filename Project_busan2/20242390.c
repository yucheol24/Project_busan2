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
int stamina; //스테미너
int cPosition; // 'C,Z,M'의 초기 위치
int zPosition;
int mPosition;
bool cMoved = false; // 시민 움직임,좀비 움직임 ,좀비(못움직이는 턴) 움직임
bool zMoved = false;
bool cannotMoved = false;
int zombieCount = 0; //좀비가 2턴마다 움직이게 하는 변수
int moveSelect; // 마동석 움직임 고르기 0,1
int turn = 0; //페이즈
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
    do { //do while문으로 값이 맞지 않으면 다시 입력받음
        printf("train length(%d~%d)>> ", LEN_MIN, LEN_MAX);
        scanf_s("%d", &train_len);
    } while (train_len < LEN_MIN || train_len > LEN_MAX);
    do {
        printf("madongseok stamina(%d~%d)>> ", STM_MIN, STM_MAX);
        scanf_s("%d", &stamina);
    } while (stamina < STM_MIN || stamina > STM_MAX);
    do {
        printf("percentile probability 'p'(%d~%d)>> ", PROB_MIN, PROB_MAX);
        scanf_s("%d", &percent);
    } while (percent < PROB_MIN || percent > PROB_MAX);
    printf("\n");

    cPosition = train_len - 6; //초기위치 설정
    zPosition = train_len - 3;
    mPosition = train_len - 2;
}

void percentage() {
    int randomNum = rand() % 100 + 1; // 1에서 100까지의 난수 생성
    // 입력 받은 확률로 cPosition-- or zPosition--
    zombieCount += 1; // 게속 1식 올라가면서 zombieCount가 홀수일 때만 Z가 이동(zPosition--)
    if (zombieCount % 2 == 1) {
        //zPosition--;
        zMoved = true;
    }
    else { //zombieCount가 짝수이면 Z는 움직이지 않고 cannot move
        zMoved = false;
        cannotMoved = true;
    }
    if (randomNum <= 100 - percent) { //100-p 확률
        //cPosition--;
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
void aggro() {
    if (cMoved) {
        cPosition--;
        if (citizen_aggro < AGGRO_MAX) citizen_aggro++;
    }
    else {
        if (citizen_aggro > AGGRO_MIN) citizen_aggro--;
    }
    if (zMoved) {
        if (citizen_aggro >= madongseok_aggro) {
            if(zPosition - 1 != cPosition)  zPosition--;
        }
        else {
            if (mPosition - 1 != zPosition) {
                zPosition++;
            }
            else zMoved = false;
        }
    }
}
void citizen_movement() {
    printf("\n\n");
    if (cMoved) { //시민이 움직이면
        //if (citizen_aggro < AGGRO_MAX) citizen_aggro++; //시민 어그로가 5보다 작으면 어그로 + 1
        printf("citizen: %d -> %d (aggro: %d -> %d)\n", cPosition + 1, cPosition, citizen_aggro - 1, citizen_aggro);
    }
    else { //움직이지 않으면
        //if (citizen_aggro > AGGRO_MIN) citizen_aggro--; //시민 어그로가 0보다 크면 어그로 - 1
        printf("citizen: stay %d (aggro: %d -> %d)\n", cPosition, citizen_aggro + 1, citizen_aggro);
    }
}

void zombie_movement() {
    if (zMoved) { //움직였을때
        printf("zombie: %d -> %d\n", zPosition + 1, zPosition);
    }
    else if (cannotMoved) { //움직이지 않는 턴
        printf("zombie: stay %d (cannot move)\n", zPosition);
    }
    else { // 움직이지 못하면
        printf("zombie: stay %d\n", zPosition);
    }
}

void madongseok_input() {
    printf("\n");
    if (mPosition - 1 == zPosition) { // 마동석이 좀비와 인접해 있으면
        do {
            printf("madongseok move(%d:stay)>> ", MOVE_STAY); //대기만 가능
            scanf_s("%d", &moveSelect);
        } while (moveSelect != MOVE_STAY);
    }
    else { //인접해 있지 않으면
        do {
            printf("madongseok move(%d:stay, %d:left)>> ", MOVE_STAY, MOVE_LEFT); //왼쪽 이동 or 대기 선택 가능
            scanf_s("%d", &moveSelect);
        } while (moveSelect < MOVE_STAY || moveSelect > MOVE_LEFT);
    }
}
void madongseok_movement() {
    if (moveSelect == MOVE_LEFT) { // 왼쪽 이동을 고르면 왼쪽으로 이동하고
        mPosition--;
        if (madongseok_aggro < AGGRO_MAX) madongseok_aggro++; // 왼쪽 이동 -> 어그로 + 1
    }
    else { //아니면 대기
        if (madongseok_aggro > AGGRO_MIN) madongseok_aggro--; // 대기 -> 어그로 - 1
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
        aggro();
        train_situation();
        
        citizen_movement(); 
        zombie_movement();
        {
            //마동석 이동 입력 및 처리
            madongseok_input();
            madongseok_movement();
            train_situation();
        }

        if (printStatus()) {
            break;
        }
    }
}