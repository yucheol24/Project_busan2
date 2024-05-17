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
int stamina;
int cPosition; // 'C,Z,M'�� �ʱ� ��ġ
int zPosition;
int mPosition;
bool cMoved = false;
bool zMoved = false;
bool cannotMoved = false;
int zombieCount = 0;
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
    do {
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

    cPosition = train_len - 6;
    zPosition = train_len - 3;
    mPosition = train_len - 2;
}

void percentage() {
    int randomNum = rand() % 100 + 1; // 1���� 100������ ���� ����
    // �Է� ���� Ȯ���� cPosition-- or zPosition--
    zombieCount += 1; // �Լ� 1�� �ö󰡸鼭 zombieCount�� Ȧ���� ���� Z�� �̵�(zPosition--)
    if (zombieCount % 2 == 1) {
        if (randomNum <= percent) {
            zPosition--;
            zMoved = true;
        }
        else zMoved = false;
    }
    else { //zombieCount�� ¦���̸� Z�� �������� �ʰ� cannot move
        zMoved = false;
        cannotMoved = true;
    }
    if (randomNum <= 100 - percent) { //100-p Ȯ��
        cPosition--;
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

void citizen_movement() {
    printf("\n\n");
    if (cMoved) {
        if (citizen_aggro < AGGRO_MAX) citizen_aggro++;
        printf("citizen: %d -> %d (aggro: %d -> %d)\n", cPosition + 1, cPosition, citizen_aggro - 1, citizen_aggro);
    }
    else {
        if (citizen_aggro > AGGRO_MIN) citizen_aggro--;
        printf("citizen: stay %d (aggro: %d -> %d)\n", cPosition, citizen_aggro + 1, citizen_aggro);
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
        train_situation();
        citizen_movement(); zombie_movement();

        if (printStatus()) {
            break;
        }
    }
}