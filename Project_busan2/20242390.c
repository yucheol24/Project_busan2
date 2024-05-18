#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#define LEN_MIN 15 //��������
#define LEN_MAX 50
#define PROB_MIN 10 //Ȯ��
#define PROB_MAX 90
#define STM_MIN 0 //������ ü��
#define STM_MAX 5
#define AGGRO_MIN 0 //��׷� ����
#define AGGRO_MAX 5

// ������ �̵� ����
#define MOVE_LEFT 1
#define MOVE_STAY 0

// ������ ���� ���
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

// ������ �ൿ
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

// ��������
int train_len; //���� ����
int percent; //Ȯ��	
int stamina; //���׹̳�
int cPosition; // 'C,Z,M'�� �ʱ� ��ġ
int zPosition;
int mPosition;
bool cMoved = false; // �ù� ������,���� ������ ,����(�������̴� ��) ������
bool zMoved = false;
bool cannotMoved = false;
int zombieCount = 0; //���� 2�ϸ��� �����̰� �ϴ� ����
int moveSelect; // ������ ������ ���� 0,1
int turn = 0; //������
// ��׷� ����
int citizen_aggro = 1;
int madongseok_aggro = 1;
// �Լ� ����
void printIntro() { 
    printf("======================================\n");
    printf("=              �λ���                =\n");
    printf("======================================\n");
    Sleep(2000);
}

void trainSetting() {
    do { //do while������ ���� ���� ������ �ٽ� �Է¹���
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

    cPosition = train_len - 6; //�ʱ���ġ ����
    zPosition = train_len - 3;
    mPosition = train_len - 2;
}

void percentage() {
    int randomNum = rand() % 100 + 1; // 1���� 100������ ���� ����
    // �Է� ���� Ȯ���� cPosition-- or zPosition--
    zombieCount += 1; // �Լ� 1�� �ö󰡸鼭 zombieCount�� Ȧ���� ���� Z�� �̵�(zPosition--)
    if (zombieCount % 2 == 1) {
        //zPosition--;
        zMoved = true;
    }
    else { //zombieCount�� ¦���̸� Z�� �������� �ʰ� cannot move
        zMoved = false;
        cannotMoved = true;
    }
    if (randomNum <= 100 - percent) { //100-p Ȯ��
        //cPosition--;
        cMoved = true;
    }
    else cMoved = false;
}

void train_situation() {
    // ���� ��� �� �ϴ� ��� ���
    printf("\n\n");
    for (int i = 0; i < train_len; i++) {
        printf("#");
    }
    printf("\n#");

    // ���� ����
    for (int i = 1; i < train_len - 1; i++) { //�������� - 1 ��ŭ �ݺ�
        if (i == cPosition) { //�ʱ� cPosition�� train_len - 6, cPosition�� �����ϸ� �� ��ġ�� ������� 
            printf("C");
        }
        else if (i == zPosition) {
            printf("Z");
        }
        else if (i == mPosition) { //���� ������ 2��°�� M���
            printf("M");
            //i += 1;
        }
        else { //�� ���� �ش�ȵǸ� ���� ���
            printf(" ");
        }
    }
    printf("#\n");
    // ���� �ϴ� ��� ���
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
    if (cMoved) { //�ù��� �����̸�
        //if (citizen_aggro < AGGRO_MAX) citizen_aggro++; //�ù� ��׷ΰ� 5���� ������ ��׷� + 1
        printf("citizen: %d -> %d (aggro: %d -> %d)\n", cPosition + 1, cPosition, citizen_aggro - 1, citizen_aggro);
    }
    else { //�������� ������
        //if (citizen_aggro > AGGRO_MIN) citizen_aggro--; //�ù� ��׷ΰ� 0���� ũ�� ��׷� - 1
        printf("citizen: stay %d (aggro: %d -> %d)\n", cPosition, citizen_aggro + 1, citizen_aggro);
    }
}

void zombie_movement() {
    if (zMoved) { //����������
        printf("zombie: %d -> %d\n", zPosition + 1, zPosition);
    }
    else if (cannotMoved) { //�������� �ʴ� ��
        printf("zombie: stay %d (cannot move)\n", zPosition);
    }
    else { // �������� ���ϸ�
        printf("zombie: stay %d\n", zPosition);
    }
}

void madongseok_input() {
    printf("\n");
    if (mPosition - 1 == zPosition) { // �������� ����� ������ ������
        do {
            printf("madongseok move(%d:stay)>> ", MOVE_STAY); //��⸸ ����
            scanf_s("%d", &moveSelect);
        } while (moveSelect != MOVE_STAY);
    }
    else { //������ ���� ������
        do {
            printf("madongseok move(%d:stay, %d:left)>> ", MOVE_STAY, MOVE_LEFT); //���� �̵� or ��� ���� ����
            scanf_s("%d", &moveSelect);
        } while (moveSelect < MOVE_STAY || moveSelect > MOVE_LEFT);
    }
}
void madongseok_movement() {
    if (moveSelect == MOVE_LEFT) { // ���� �̵��� ���� �������� �̵��ϰ�
        mPosition--;
        if (madongseok_aggro < AGGRO_MAX) madongseok_aggro++; // ���� �̵� -> ��׷� + 1
    }
    else { //�ƴϸ� ���
        if (madongseok_aggro > AGGRO_MIN) madongseok_aggro--; // ��� -> ��׷� - 1
    }
}

bool printStatus() {
    // C�� ���� �����ϸ� Ż�� ���� ��� �� ���α׷� ����
    if (cPosition == 1) { // ���� 1
        printf("\nSUCCESS!\n");
        printf("citizen(s) escaped to the next train\n");
        return true;
    }
    // Z�� C�ٷ� ���� �����ϸ� ���� ���� ��� �� ���α׷� ����
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
            //������ �̵� �Է� �� ó��
            madongseok_input();
            madongseok_movement();
            train_situation();
        }

        if (printStatus()) {
            break;
        }
    }
}