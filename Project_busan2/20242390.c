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
int before_zPosition; //zposition ���� �� ��
int before_cPosition; //cposition ���� �� ��
int before_aggro_C; //
int before_stamina; //���׹̳� �� ��
bool cMoved = false; // �ù� ������,���� ������ ,����(�������̴� ��) ������
bool zMoved = false;
bool cannotMoved = false;
bool mMoved = false;
bool zombie_attcked_madongseok = false;
bool Z_near_C = false;
bool zmoved_citizen = false;
bool zmoved_madongseok = false;
bool Pull_cannotMoved = false;
int zombieCount = 0; //���� 2�ϸ��� �����̰� �ϴ� ����
int moveSelect; // ������ ������ ���� 0,1
int actionSelect; // ������ �޽�/���� ����
// ��׷� ����
int citizen_aggro = 1;
int madongseok_aggro = 1;
// 1-1 �Լ� ����
void printIntro() { 
    printf("========================================\n");
    printf("|                                      |\n");
    printf("|              �λ���                  |\n");
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
// 1-2 ��������
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
// 1-3 Ȯ������
void percentage() {
    int randomNum = rand() % 100; // 1���� 100������ ���� ����
    // �Է� ���� Ȯ���� cPosition-- or zPosition--
    zombieCount += 1; // �Լ� 1�� �ö󰡸鼭 zombieCount�� Ȧ���� ���� Z�� �̵�(zPosition--)
    if (zombieCount % 2 == 1 && !Pull_cannotMoved) {
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
// 1-4 ���� ����Ȳ
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
//1-5 ��׷� ����
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
        before_zPosition = zPosition; // zposition ���� �� ��
        if (citizen_aggro >= madongseok_aggro) { //�ùξ�׷ΰ� ��������׷κ��� ũ�ų� ������
            if(zPosition - 1 != cPosition) zPosition--; //���� �ù��̶� ������ ���� ������ ����, �� ������ ������ ����� �������� �������� ����
        }
        else { //�ùξ�׷ΰ� ��������׷κ��� ������,
            if (zPosition + 1 != mPosition) zPosition++; //  ���� �������̶� ������ ���� ������z++, �� ������ ������ ���������� �������� ����
        }
    }
}
// 1-6 �ù� ������ ����
void citizen_movement() {
    printf("\n\n");
    if (cMoved) { //�ù��� �����̸�

        printf("citizen: %d -> %d (aggro: %d -> %d)\n", before_cPosition, cPosition, before_aggro_C, citizen_aggro);
    }
    else { //�������� ������
        printf("citizen: stay %d (aggro: %d -> %d)\n", cPosition, before_aggro_C, citizen_aggro);
    }
}
// 1-7 ���� ������ ����
void zombie_movement() {
    if (zMoved && !Pull_cannotMoved) { //����������, Pull_cannotMoved�� �ƴҶ�..
        printf("zombie: %d -> %d\n", before_zPosition, zPosition);
    }
    else if (cannotMoved || Pull_cannotMoved) { //�������� �ʴ� ���̰ų� ������ ���������� �������� ���� ��
        printf("zombie: stay %d (cannot move)\n", zPosition);
    }
    else { // �������� ���ϸ�
        printf("zombie: stay %d\n", zPosition);
    }
}
// 1-8 ������ ������ �Է�
void madongseok_input() {
    printf("\n");
    if (mPosition - 1 == zPosition) { // �������� ����� ������ ������ 
        do {
            printf("madongseok move(%d:stay)>> ", MOVE_STAY); //��⸸ ����, �Է� ����
            scanf_s("%d", &moveSelect);
        } while (moveSelect != MOVE_STAY); //���ϴ� ���� �ƴϸ� �Լ� �Է¹���
    }
    else { //������ ���� ������
        do {
            printf("madongseok move(%d:stay, %d:left)>> ", MOVE_STAY, MOVE_LEFT); //���� �̵� or ��� ���� ����
            scanf_s("%d", &moveSelect);
        } while (moveSelect < MOVE_STAY || moveSelect > MOVE_LEFT); //���� ����
    }
    if (moveSelect == 1) { //������ �����̸� 
        mMoved = true; //������
    }
    else mMoved = false; //�ƴϸ� �� ������
}
// 1-9 ������ ������ ����
void madongseok_movement() { //������ ������
    if (mMoved) { // ���� �̵��� ���� �������� �̵��ϰ�
        mPosition--; //�������� �̵�
        if (madongseok_aggro < AGGRO_MAX) madongseok_aggro++; // ���� �̵� -> ��׷� + 1
    }
    else { //�ƴϸ� ���
        if (madongseok_aggro > AGGRO_MIN) madongseok_aggro--; // ��� -> ��׷� - 1
    }
}
// 2-1 ������ ������ ���
void madongseok_result() { //������ ������ ���
    if (mMoved) { //������ �����̸�
        printf("\nmadongseok: stay %d(aggro: %d -> %d, stamina: %d)", mPosition, madongseok_aggro - 1, madongseok_aggro, stamina); 
    }
    else {
        printf("\nmadongseok: stay %d(aggro: %d -> %d, stamina: %d)", mPosition, madongseok_aggro + 1, madongseok_aggro, stamina);
    }
}
// 2-2 �ù� ���� ���
void citizen_does_noting() { //�ù��� �ƹ��͵� ��������
    printf("\n\n");
    printf("citizen does nothing.\n"); 
}
// 2-3 ���� ���� ����
void zombie_attcked() { //���� ����
    zombie_attcked_madongseok = false; //���� ������ ������ �� ����
    Z_near_C = false; // ���� �ùΰ� ������ �� ����
    if ((cPosition == zPosition - 1) && (zPosition == mPosition - 1)) { // ���� ������ ���� ��
        if (citizen_aggro > madongseok_aggro) { //�ù� ��׷ΰ� ������ ��׷� ���� ũ��
            Z_near_C = true; //���� �ùΰ� ����
        }
        else zombie_attcked_madongseok = true; //�ƴϸ� ���� �������� ����
    }
    else if (cPosition == zPosition - 1) { //���� ������ ���� �ʰ� �ùΰ� ���� ������ ������ 
        Z_near_C = true; //���� �ùΰ� ����
    }
    else if (zPosition == mPosition - 1) { //����� �������� ������ ������
        zombie_attcked_madongseok = true; //���� �������� ����
    }
    else {
        printf("\nzombie attacked nobody.\n"); //��� �ƴϸ� ����� ���� ����
    }
}
// 2-4 ���� ���� ���� 2
void zombie_attcked_2() {  //���� ���� 2 //������ ����
    before_stamina = stamina;
    if(stamina > STM_MIN) stamina--; //���׹̳� -1

    printf("Zombie attcked madongseok (aggro: %d vs .%d, madongseok stamina: %d -> %d)\n", citizen_aggro, madongseok_aggro, before_stamina, stamina);
}
// 2-5 ��������
void gameover() { //���� ��
    if (stamina == STM_MIN) { //���׹̳ʰ� 0�� �Ǹ� ����
        printf("\nGAME OVER!\n");
        exit(0);
    }
}
// 2-6 �������� 2
bool printStatus() { //���� ��
    // C�� ���� �����ϸ� Ż�� ���� ��� �� ���α׷� ����
    if (cPosition == 1) { // ���� 1
        printf("\nYOU WIN! citizen(s) escaped to the next train\n");
        return true;
    }
    // Z�� C�ٷ� ���� �����ϸ� ���� ���� ��� �� ���α׷� ����
    if (Z_near_C) {
        printf("\nGAME OVER! citizen dead...\n");
        printf("citizen(s) has(have) been attacked by a zombie\n");
        return true;
    }
    return false;
}
// 2-7 ������ �ൿ ����
void madongseok_action() { //������ �ൿ
    if (zPosition != mPosition - 1) { //����� �������� ���� ���(������ 2��)
        do {
            printf("madongseok action(%d.rest, %d.provoke)>> ", ACTION_REST, ACTION_PROVOKE);
            scanf_s("%d", &actionSelect);
        } while (actionSelect != ACTION_REST && actionSelect != ACTION_PROVOKE);
    }
    else { //����� ������ ���(������ 3��)
        do {
            printf("madongseok action(%d.rest, %d.provoke, %d. pull)>> ", ACTION_REST, ACTION_PROVOKE, ACTION_PULL);
            scanf_s("%d", &actionSelect);
        } while (actionSelect != ACTION_REST && actionSelect != ACTION_PROVOKE && actionSelect != ACTION_PULL);
    }

    if (actionSelect == ACTION_REST) { //����
        if(madongseok_aggro > AGGRO_MIN) madongseok_aggro--;  //������ ��׷� -1
        before_stamina = stamina; // �� ��
        if(stamina < STM_MAX) stamina++; //���׹̳� +1
        printf("\nmadongseok rests...\n");
        printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", mPosition, madongseok_aggro + 1, madongseok_aggro, before_stamina, stamina);
    }
    if (actionSelect == ACTION_PROVOKE) { //����
        printf("\nmadongseok provoked zombie...\n");
        printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", mPosition, madongseok_aggro, AGGRO_MAX, stamina); //��׷� -> 5
        madongseok_aggro = AGGRO_MAX;
    }
}
// 2-8 ������ �ൿ ��� ���
void madongseok_action_result() {
    Pull_cannotMoved = false;
    int previous_aggro = madongseok_aggro;
    int randomNum_Pull = rand() % 100; //100 - p Ȯ���� 
    if (actionSelect == ACTION_PULL) { //�ٵ��
        if (randomNum_Pull <= 100 - percent) { //100 - p Ȯ���� ����
            printf("\nmadongseok pulled zombie... Next turn, it can't move\n"); //�����ϸ� ���
            Pull_cannotMoved = true; 
        }
        else {
            printf("\nmadongseok failed to pull zombie\n"); //�����ϸ� ���
        }
        before_stamina = stamina; //�� ��
        if (stamina > STM_MIN) stamina--; //ü�� 1����

        if (madongseok_aggro < AGGRO_MAX) { //��������׷ΰ� 5���� ������
            madongseok_aggro += 2; //��׷� 2����
            if (madongseok_aggro > AGGRO_MAX) { //��������׷ΰ� 5���� Ŀ����
                madongseok_aggro = AGGRO_MAX; //�������� // 5���� Ŭ �� ����
            }
        }
        printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)", mPosition, previous_aggro, madongseok_aggro, before_stamina, stamina); //���
    }
}
int main(void) {
    srand((unsigned int)time(NULL));
    printIntro(); //��Ʈ��
    trainSetting(); //���� �ʱ� ����
    train_situation(); //���� ����Ȳ
    while (1) {
        cMoved = false;
        zMoved = false;
        cannotMoved = false;
        mMoved = false;
        percentage(); //Ȯ����
        aggro(); //��׷� 
        train_situation(); //���� ����Ȳ
        citizen_movement(); //�ù� ������
        zombie_movement(); //���� ������
        {
            //������ �̵� �Է� �� ó��
            madongseok_input();
            //������ ������
            madongseok_movement();
            //���� ��Ȳ
            train_situation();
            //������ ���
            madongseok_result();
        }
        if (cPosition != 1) { //�ù��� ���� �������� ���ϸ� 
            citizen_does_noting(); //�ƹ��͵� ��������
        }
        zombie_attcked(); //���� ����
        if (zombie_attcked_madongseok) { //���� �������� �����ϸ� 
            zombie_attcked_2(); //���� ����2 //���׹̳� ����
        }
        gameover(); //���� ��
        madongseok_action(); //������ �ൿ
        madongseok_action_result(); //������ ���
        if (printStatus()) { //���� ��
            break;
        }
    }
}