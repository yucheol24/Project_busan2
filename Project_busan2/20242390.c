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
int before_zPosition; //zposition 변경 전 값
int before_cPosition; //cposition 변경 전 값
int before_aggro_C; //
int before_stamina; //스테미너 전 값
bool cMoved = false; // 시민 움직임,좀비 움직임 ,좀비(못움직이는 턴) 움직임
bool zMoved = false;
bool cannotMoved = false;
bool mMoved = false;
bool zombie_attcked_madongseok = false;
bool Z_near_C = false;
bool zmoved_citizen = false;
bool zmoved_madongseok = false;
bool Pull_cannotMoved = false;
int zombieCount = 0; //좀비가 2턴마다 움직이게 하는 변수
int moveSelect; // 마동석 움직임 고르기 0,1
int actionSelect; // 마동석 휴식/도발 고르기
// 어그로 변수
int citizen_aggro = 1;
int madongseok_aggro = 1;
// 1-1 함수 선언
void printIntro() { 
    printf("========================================\n");
    printf("|                                      |\n");
    printf("|              부산행                  |\n");
    printf("|                                      |\n");
    printf("========================================\n");
    printf("        _____                          \n");
    printf("   ___|[ ]|___                        \n");
    printf("  |          |                        \n");
    printf("  |    ___   |                        \n");
    printf(" _|___|___|__|____                    \n");
    printf("|_________________|____               \n");
    printf(" 0----0----0----O----0----O               \n");
    printf("========================================\n");
    Sleep(2000);
}
// 1-2 기차세팅
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
// 1-3 확률세팅
void percentage() {
    int randomNum = rand() % 100; // 1에서 100까지의 난수 생성
    // 입력 받은 확률로 cPosition-- or zPosition--
    zombieCount += 1; // 게속 1식 올라가면서 zombieCount가 홀수일 때만 Z가 이동(zPosition--)
    if (zombieCount % 2 == 1 && !Pull_cannotMoved) {
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
// 1-4 기차 현상황
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
//1-5 어그로 세팅
void aggro() {
    before_aggro_C = citizen_aggro;
    if (cMoved) {

        before_cPosition = cPosition;
        cPosition--;
        if (citizen_aggro < AGGRO_MAX) {
            citizen_aggro++;
            if (citizen_aggro >= AGGRO_MAX) citizen_aggro = AGGRO_MAX;
        }
    }
    else {
        if (citizen_aggro > AGGRO_MIN) {
            citizen_aggro--;
            if (citizen_aggro <= AGGRO_MIN) citizen_aggro = AGGRO_MIN;
        }
    }
    if (zMoved) {
        before_zPosition = zPosition; // zposition 변경 전 값
        if (citizen_aggro >= madongseok_aggro) { //시민어그로가 마동석어그로보다 크거나 같으면
            if(zPosition - 1 != cPosition) zPosition--; //좀비가 시민이랑 인접해 있지 않으면 감소, 즉 인접해 있으면 좀비는 왼쪽으로 움직이지 않음
        }
        else { //시민어그로가 마동석어그로보다 작으면,
            if (zPosition + 1 != mPosition) zPosition++; //  좀비가 마동석이랑 인접해 있지 않으면z++, 즉 인접해 있으면 오른쪽으로 움직이지 않음
        }
    }
}
// 1-6 시민 움직임 세팅
void citizen_movement() {
    printf("\n\n");
    if (cMoved) { //시민이 움직이면

        printf("citizen: %d -> %d (aggro: %d -> %d)\n", before_cPosition, cPosition, before_aggro_C, citizen_aggro);
    }
    else { //움직이지 않으면
        printf("citizen: stay %d (aggro: %d -> %d)\n", cPosition, before_aggro_C, citizen_aggro);
    }
}
// 1-7 좀비 움직임 세팅
void zombie_movement() {
    if (zMoved && !Pull_cannotMoved) { //움직였을때, Pull_cannotMoved가 아닐때..
        printf("zombie: %d -> %d\n", before_zPosition, zPosition);
    }
    else if (cannotMoved || Pull_cannotMoved) { //움직이지 않는 턴이거나 마동석 붙잡임으로 움직이지 못할 때
        printf("zombie: stay %d (cannot move)\n", zPosition);
    }
    else { // 움직이지 못하면
        printf("zombie: stay %d\n", zPosition);
    }
}
// 1-8 마동석 움직임 입력
void madongseok_input() {
    printf("\n");
    if (mPosition - 1 == zPosition) { // 마동석이 좀비와 인접해 있으면 
        do {
            printf("madongseok move(%d:stay)>> ", MOVE_STAY); //대기만 가능, 입력 받음
            scanf_s("%d", &moveSelect);
        } while (moveSelect != MOVE_STAY); //원하는 값이 아니면 게속 입력받음
    }
    else { //인접해 있지 않으면
        do {
            printf("madongseok move(%d:stay, %d:left)>> ", MOVE_STAY, MOVE_LEFT); //왼쪽 이동 or 대기 선택 가능
            scanf_s("%d", &moveSelect);
        } while (moveSelect < MOVE_STAY || moveSelect > MOVE_LEFT); //위와 같음
    }
    if (moveSelect == 1) { //마동석 움직이면 
        mMoved = true; //움직임
    }
    else mMoved = false; //아니면 안 움직임
}
// 1-9 마동석 움직임 세팅
void madongseok_movement() { //마동석 움직임
    if (mMoved) { // 왼쪽 이동을 고르면 왼쪽으로 이동하고
        mPosition--; //왼쪽으로 이동
        if (madongseok_aggro < AGGRO_MAX) madongseok_aggro++; // 왼쪽 이동 -> 어그로 + 1
    }
    else { //아니면 대기
        if (madongseok_aggro > AGGRO_MIN) madongseok_aggro--; // 대기 -> 어그로 - 1
    }
}
// 2-1 마동석 움직임 결과
void madongseok_result() { //마동석 움직임 결과
    if (mMoved) { //마동석 움직이면
        printf("\nmadongseok: stay %d(aggro: %d -> %d, stamina: %d)", mPosition, madongseok_aggro - 1, madongseok_aggro, stamina); 
    }
    else {
        printf("\nmadongseok: stay %d(aggro: %d -> %d, stamina: %d)", mPosition, madongseok_aggro + 1, madongseok_aggro, stamina);
    }
}
// 2-2 시민 안함 출력
void citizen_does_noting() { //시민이 아무것도 하지않음
    printf("\n\n");
    printf("citizen does nothing.\n"); 
}
// 2-3 좀비 공격 세팅
void zombie_attcked() { //좀비 공격
    zombie_attcked_madongseok = false; //좀비가 마동석 공격할 때 변수
    Z_near_C = false; // 좀비가 시민과 인접할 때 변수
    if ((cPosition == zPosition - 1) && (zPosition == mPosition - 1)) { // 서로 인접해 있을 때
        if (citizen_aggro > madongseok_aggro) { //시민 어그로가 마동석 어그로 보다 크면
            Z_near_C = true; //좀비가 시민과 인접
        }
        else zombie_attcked_madongseok = true; //아니면 좀비가 마동석을 공격
    }
    else if (cPosition == zPosition - 1) { //서로 인접해 있지 않고 시민과 좀비만 인접해 있으면 
        Z_near_C = true; //좀비가 시민과 인접
    }
    else if (zPosition == mPosition - 1) { //좀비와 마동석만 인접해 있으면
        zombie_attcked_madongseok = true; //좀비가 마동석을 공격
    }
    else {
        printf("\nzombie attacked nobody.\n"); //모두 아니면 좀비는 공격 안함
    }
}
// 2-4 좀비 공격 세팅 2
void zombie_attcked_2() {  //좀비 공격 2 //마동석 공격
    before_stamina = stamina;
    if(stamina > STM_MIN) stamina--; //스테미너 -1

    printf("Zombie attcked madongseok (aggro: %d vs .%d, madongseok stamina: %d -> %d)\n", citizen_aggro, madongseok_aggro, before_stamina, stamina);
}
// 2-5 게임종료
void gameover() { //게임 끝
    if (stamina == STM_MIN) { //스테미너가 0이 되면 종료
        printf("\nGAME OVER!\n");
        exit(0);
    }
}
// 2-6 게임종료 2
bool printStatus() { //게임 끝
    // C가 끝에 도착하면 탈출 성공 출력 후 프로그램 종료
    if (cPosition == 1) { // 끝이 1
        printf("\nYOU WIN! citizen(s) escaped to the next train\n");
        return true;
    }
    // Z가 C바로 옆에 도착하면 구출 실패 출력 후 프로그램 종료
    if (Z_near_C) {
        printf("\nGAME OVER! citizen dead...\n");
        printf("citizen(s) has(have) been attacked by a zombie\n");
        return true;
    }
    return false;
}
// 2-7 마동석 행동 세팅
void madongseok_action() { //마동석 행동
    if (zPosition != mPosition - 1) { //좀비와 인접하지 않은 경우(선택지 2개)
        do {
            printf("madongseok action(%d.rest, %d.provoke)>> ", ACTION_REST, ACTION_PROVOKE);
            scanf_s("%d", &actionSelect);
        } while (actionSelect != ACTION_REST && actionSelect != ACTION_PROVOKE);
    }
    else { //좀비와 인접한 경우(선택지 3개)
        do {
            printf("madongseok action(%d.rest, %d.provoke, %d. pull)>> ", ACTION_REST, ACTION_PROVOKE, ACTION_PULL);
            scanf_s("%d", &actionSelect);
        } while (actionSelect != ACTION_REST && actionSelect != ACTION_PROVOKE && actionSelect != ACTION_PULL);
    }

    if (actionSelect == ACTION_REST) { //쉬기
        if(madongseok_aggro > AGGRO_MIN) madongseok_aggro--;  //마동석 어그로 -1
        before_stamina = stamina; // 전 값
        if(stamina < STM_MAX) stamina++; //스테미너 +1
        printf("\nmadongseok rests...\n");
        printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", mPosition, madongseok_aggro + 1, madongseok_aggro, before_stamina, stamina);
    }
    if (actionSelect == ACTION_PROVOKE) { //도발
        printf("\nmadongseok provoked zombie...\n");
        printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", mPosition, madongseok_aggro, AGGRO_MAX, stamina); //어그로 -> 5
        madongseok_aggro = AGGRO_MAX;
    }
}
// 2-8 마동석 행동 결과 출력
void madongseok_action_result() {
    Pull_cannotMoved = false;
    int previous_aggro = madongseok_aggro;
    int randomNum_Pull = rand() % 100; //100 - p 확률로 
    if (actionSelect == ACTION_PULL) { //붙들기
        if (randomNum_Pull <= 100 - percent) { //100 - p 확률로 성공
            printf("\nmadongseok pulled zombie... Next turn, it can't move\n"); //성공하면 출력
            Pull_cannotMoved = true; 
        }
        else {
            printf("\nmadongseok failed to pull zombie\n"); //실패하면 출력
        }
        before_stamina = stamina; //전 값
        if (stamina > STM_MIN) stamina--; //체력 1감소

        if (madongseok_aggro < AGGRO_MAX) { //마동석어그로가 5보다 작으면
            madongseok_aggro += 2; //어그로 2증가
            if (madongseok_aggro > AGGRO_MAX) { //마동석어그로가 5보다 커지면
                madongseok_aggro = AGGRO_MAX; //같아지게 // 5보다 클 수 없음
            }
        }
        printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)", mPosition, previous_aggro, madongseok_aggro, before_stamina, stamina); //결과
    }
}
int main(void) {
    srand((unsigned int)time(NULL));
    printIntro(); //인트로
    trainSetting(); //열차 초기 세팅
    train_situation(); //열차 현상황
    while (1) {
        cMoved = false;
        zMoved = false;
        cannotMoved = false;
        mMoved = false;
        percentage(); //확률들
        aggro(); //어그로 
        train_situation(); //열차 현상황
        citizen_movement(); //시민 움직임
        zombie_movement(); //좀비 움직임
        {
            //마동석 이동 입력 및 처리
            madongseok_input();
            //마동석 움직임
            madongseok_movement();
            //기차 상황
            train_situation();
            //마동석 결과
            madongseok_result();
        }
        if (cPosition != 1) { //시민이 끝에 도착하지 못하면 
            citizen_does_noting(); //아무것도 하지않음
        }
        zombie_attcked(); //좀비 공격
        if (zombie_attcked_madongseok) { //좀비가 마동석을 공격하면 
            zombie_attcked_2(); //좀비 공격2 //스테미너 감소
        }
        gameover(); //게임 끝
        madongseok_action(); //마동석 행동
        madongseok_action_result(); //마동석 결과
        if (printStatus()) { //게임 끝
            break;
        }
    }
}