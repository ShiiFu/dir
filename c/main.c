#include <stdio.h>
#include <windows.h>

void dir (char *path, int level);
void tab(int level);

int main (int argc, char *argv[])
{
  printf("Fonction windows DIR /S en C\n\n");

  // Appel de l'initialisation de la fonction dir récursive avec le chemin du dossier cible
  if (argc != 2) {
    dir(".", 0); // "." si aucun chemin n'est spécifié en paramètre
  }
  else {
    dir(argv[1], 0); // le chemin spécifié sinon
  }

  return 0;
}

void dir(char *path, int level) {
  WIN32_FIND_DATA FindFileData;
  HANDLE hFind;
  LARGE_INTEGER filesize;
  TCHAR szDir[MAX_PATH];

  SYSTEMTIME st;
  char szLocalDate[255], szLocalTime[255];

  strcpy(szDir, path);
  strcat(szDir, TEXT("\\*"));

  // Find first file (current dir)
  hFind = FindFirstFile(szDir, &FindFileData);

  do {
    // Format creation date of the file
    FileTimeToLocalFileTime(&FindFileData.ftLastWriteTime, &FindFileData.ftLastWriteTime);
    FileTimeToSystemTime(&FindFileData.ftLastWriteTime, &st);
    GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, NULL, szLocalDate, 255);
    GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &st, NULL, szLocalTime, 255);

    // Print tab with recursivity level
    tab(level);

    if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      // Print directory informations
      printf("%s  %s    <DIR>          %s\n", szLocalDate, szLocalTime, FindFileData.cFileName);

      // If not . or .. directory, call recursivity
      if (strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0) {
        char new_path [255];
        strcpy(new_path, path);
        strcat(new_path, "\\");
        strcat(new_path, FindFileData.cFileName);
        dir(new_path, level + 1);
      }
    }
    else {
      // Print file informations
      filesize.LowPart = FindFileData.nFileSizeLow;
      filesize.HighPart = FindFileData.nFileSizeHigh;
      printf("%s  %s %*lld %s\n", szLocalDate, szLocalTime, 17, filesize.QuadPart, FindFileData.cFileName);
    }
  }
  while (FindNextFile(hFind, &FindFileData) != 0);
}

void tab(int level) {
  for (int i=0 ; i<level ; i++) {
    printf("\t");
  }
}
