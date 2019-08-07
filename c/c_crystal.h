#pragma once

typedef struct C_Room C_Room;

C_Room * newRoom(int x, int y, int z, int type);

void inputECC(C_Room * room, int num, int length);
void input_one_ECC(int x, int y, int z, int length, int direction, int* type, int moveable);
void movie(C_Room * room, int m, int n, double T);
void input_one_FCC(C_Room * room, int x, int y, int z, int length, int direction, int fold_direction, int* type, int moveable);
void movie(C_Room * room, int m, int n, double T);
void DeleteRoom(C_Room * room);