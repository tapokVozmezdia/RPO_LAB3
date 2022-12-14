#define _GNU_SOURCE
#include "lab3.h"
#include <stdio.h>
#include <string.h>

typedef enum
{
    E_INVALID = 0,
    E_ADD_GROUP,
    E_ADD_STUDENT,
    E_DEL_GROUP,
    E_DEL_STUDENT,
    E_PRINT_UNI,
    E_PRINT_GROUP,
    E_PRINT_STUDENT,
    E_END
} EAction;

const char *Enum2Str(EAction eAct)
{
    switch (eAct)
    {
        case E_INVALID:
            return "- Невалидная команда";
        case E_ADD_GROUP:
            return "- Добавить группу";
        case E_ADD_STUDENT:
            return "- Добавить студента";
        case E_DEL_GROUP:
            return "- Удалить группу";
        case E_DEL_STUDENT:
            return "- Удалить студента";
        case E_PRINT_UNI:
            return "- Напечатать информацию об университете";
        case E_PRINT_GROUP:
            return "- Напечатать информацию о группе";
        case E_PRINT_STUDENT:
            return "- Напечатать информацию о студенте";
        case E_END:
            return "- Окончание работы";
        default:
            return "? - Невалидная команда";
    }
}

Group *addGr(University *university) {

    Group *group = NULL;
    char name[SIZE];

    printf("Enter the name of the new group:\n");
    scanf("%s", name);

    group = (Group*)malloc(sizeof(Group));
    if (group == NULL) {
        return NULL;
    }

    strcpy(group->name, name);
    group->studentsCount = 0;
    addNewGroup(university, *group);
    
    return group;
}

void addSt(University *university) {
	
    Student *student = NULL;
    int groupId = 0;
    int groupCheck = 0;
    Group *group = NULL;
    student = (Student *) malloc(sizeof(Student));

    printf("Enter student's name:\n");
    scanf("%s", student->name);
    printf("Enter student's surname:\n");
    scanf("%s", student->surname);
    printf("Enter student's year of birth\n");
    scanf("%d", &student->birthYear);
    printf("Enter student's group:\n");
    for (int i = 0; i < university->groupsCount; ++i) {
        printf("%d %s\n", i + 1, university->groups[i].name);
    }
	
    printf("%d %s\n", university->groupsCount + 1, "is a new group");
    scanf("%d", &groupId);

    if (groupId == university->groupsCount + 1) {
        group = addGr(university);
	groupCheck = 1;
    } 
	else {
        group = &university->groups[groupId - 1];
    }
    addNewStudent(group, *student);
    free(student);
    if (groupCheck) {
        free(group);
    }
}

int main(int argc, char* argv[])
{

    char *fileName = NULL;
    size_t fileSize = 0;

    if (argc < 2) {
        printf("Not given path to university file, manual input is required:\n");
        getline(&fileName, &fileSize, stdin);
        fileName[strlen(fileName) - 1] = '\0';
    }
    else {
        fileName = argv[1];
    }

	EAction eAct = E_INVALID;

	University *university = initUniversity(fileName);
    Group *group = NULL;
    Student *student = NULL;

    char groupName[SIZE];
    int groupId = 0;
    int flag = 0;
    while (eAct != E_END)
	{
		int chosenAction;

        printf("Available actions:\n");

        for (int i = 1; i <= E_END; ++i) {
            printf("%d %s\n", i, Enum2Str((EAction)i));
        }

		scanf("%d", &chosenAction);

        if (chosenAction >= 1 && chosenAction <= E_END) {
            eAct = (EAction)chosenAction;
        }
        else {
            eAct = E_INVALID;
        }

		switch (eAct)
		{
			case E_ADD_GROUP:

                group = addGr(university);
                free(group);
                continue;

            case E_ADD_STUDENT:
           		addSt(university);	
                continue;

            case E_DEL_GROUP:

                memset(groupName, 0, SIZE);

                printf("Enter name of the group you wish to delete:\n");
                scanf("%s", groupName);

                if (removeGroup(university, groupName)) {
                    printf("Group has been deleted\n");
                }
                else {
                    printf("Group has not been found\n");
                }

                continue;

            case E_DEL_STUDENT:

                printf("Enter student's ID:\n");
                scanf("%d", &groupId);

                if (removeStudent(university, groupId)) {
                    printf("Student has been deleted\n");
                }
                else {
                    printf("Student has not been found\n");
                }

                continue;

            case E_PRINT_UNI:

                printUniversity(university);
                continue;

            case E_PRINT_GROUP:

                memset(groupName, 0, SIZE);

                printf("Enter the name of the group:\n");
                scanf("%s", groupName);
                group = getGroup(university, groupName);
                if (group == NULL) {
                    printf("Group has not been found\n");
                }
                else {
                    printGroup(*group);
                }

                continue;

            case E_PRINT_STUDENT:

                printf("Enter student's ID:\n");
                scanf("%d", &groupId);
                student = getStudent(university, groupId);
                if (student == NULL) {
                    printf("Student has not been found\n");
                }
                else {
                    printStudent(*student);
                }

                continue;

            case E_INVALID:

                printf("Invalid command\n");

                continue;

            case E_END:
                break;
		}
	}
	char *saveFileName = NULL;
    size_t saveFileSize = 0;
	printf("Program finished\n");
    printf("Enter the name of the file to save:\n");
    fflush(stdin);
    fflush(stdout);
	
    int s;
    do {
        s = getchar();
    } while (s != '\n' && s != EOF);
	
    getline(&saveFileName, &saveFileSize, stdin);
    saveFileName[strlen(saveFileName) - 1] = '\0';
    printf("File saved. New path: %s\n", saveFileName);
    saveToFile(saveFileName, university);
    freeUniversity(university);
	free(fileName);
    free(saveFileName);

    return 0;
}
