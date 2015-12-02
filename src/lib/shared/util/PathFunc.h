#ifndef SHARED_PATHFUNC_H
#define SHARED_PATHFUNC_H


void GetExePath(char* path, int size);

bool myCreateDir(const char* fullpath);
bool myCreateFile(const char* name);


#endif	//SHARED_PATHFUNC_H
