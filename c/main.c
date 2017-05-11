#include <stdio.h>
#include <windows.h>

void dir (char *path);
void dir_r(char *path, int level);
void tab(int level);

int main (int argc, char *argv[])
{
  printf("Fonction windows DIR /S en C\n\n");

  if (argc != 2) {
    dir(".");
  }
  else {
    dir(argv[1]);
  }

  return 0;
}

void dir (char *path) {

  // Le volume dans le lecteur C s'appelle OS
  // Le numéro de série du volume est XXXX-XXXX

  printf(" Repertoire de %s\n", path); // Chemin relatif à remplacer par absolu

  WIN32_FIND_DATA FindFileData;
  HANDLE hFind;
  LARGE_INTEGER filesize;
  TCHAR szDir[MAX_PATH];

  SYSTEMTIME st;
  char szLocalDate[255], szLocalTime[255];

  strcpy(szDir, path);
  strcat(szDir, TEXT("\\*"));

  hFind = FindFirstFile(szDir, &FindFileData);

  int nFile = 0;
  int nDir = 0;
  int fileSpace = 0;
  int freeSpace = 0;

  do {
    FileTimeToLocalFileTime(&FindFileData.ftLastWriteTime, &FindFileData.ftLastWriteTime);
    FileTimeToSystemTime(&FindFileData.ftLastWriteTime, &st);
    GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, NULL, szLocalDate, 255);
    GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &st, NULL, szLocalTime, 255);
    if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      printf("%s  %s    <DIR>          %s\n", szLocalDate, szLocalTime, FindFileData.cFileName);
      nDir++;
      if (strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0) {
        char new_path [255];
        strcpy(new_path, path);
        strcat(new_path, "\\");
        strcat(new_path, FindFileData.cFileName);
        dir_r(new_path, 1);
      }
    }
    else {
      filesize.LowPart = FindFileData.nFileSizeLow;
      filesize.HighPart = FindFileData.nFileSizeHigh;
      printf("%s  %s %*lld %s\n", szLocalDate, szLocalTime, 17, filesize.QuadPart, FindFileData.cFileName);
      nFile++;
      fileSpace += filesize.QuadPart;
    }
  }
  while (FindNextFile(hFind, &FindFileData) != 0);

  printf("%*d fichier(s)     %d octets\n", 15, nFile, fileSpace);
  printf("%*d Rep(s)     %d octets libres\n", 15, nDir, freeSpace);

  FindClose(hFind);
}


void dir_r(char *path, int level) {
  WIN32_FIND_DATA FindFileData;
  HANDLE hFind;
  LARGE_INTEGER filesize;
  TCHAR szDir[MAX_PATH];

  SYSTEMTIME st;
  char szLocalDate[255], szLocalTime[255];

  strcpy(szDir, path);
  strcat(szDir, TEXT("\\*"));

  hFind = FindFirstFile(szDir, &FindFileData);

  int nFile = 0;
  int nDir = 0;
  int fileSpace = 0;
  int freeSpace = 0;

  do {
    FileTimeToLocalFileTime(&FindFileData.ftLastWriteTime, &FindFileData.ftLastWriteTime);
    FileTimeToSystemTime(&FindFileData.ftLastWriteTime, &st);
    GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, NULL, szLocalDate, 255);
    GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &st, NULL, szLocalTime, 255);
    tab(level);
    if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      printf("%s  %s    <DIR>          %s\n", szLocalDate, szLocalTime, FindFileData.cFileName);
      nDir++;
      if (strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0) {
        char new_path [255];
        strcpy(new_path, path);
        strcat(new_path, "\\");
        strcat(new_path, FindFileData.cFileName);
        dir_r(new_path, level + 1);
      }
    }
    else {
      filesize.LowPart = FindFileData.nFileSizeLow;
      filesize.HighPart = FindFileData.nFileSizeHigh;
      printf("%s  %s %*lld %s\n", szLocalDate, szLocalTime, 17, filesize.QuadPart, FindFileData.cFileName);
      nFile++;
      fileSpace += filesize.QuadPart;
    }
  }
  while (FindNextFile(hFind, &FindFileData) != 0);
}

void tab(int level) {
  for (int i=0 ; i<level ; i++) {
    printf("\t");
  }
}
