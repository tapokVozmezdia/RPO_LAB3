#include "lab3.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

unsigned long int g_Id = 1;

Student *getStudent(const University *university, const unsigned long id) {

    if (university == NULL) {
        return NULL;
    }

    for (int i = 0; i < university->groupsCount; ++i) {

        for (int j = 0; j < university->groups[i].studentsCount; ++j) {

            if (id == university->groups[i].students[j].id) {
                return &university->groups[i].students[j];
            }

        }

    }

    return NULL;
}

Group *getGroup(const University *university, const char *name) {

    if (university != NULL && name!= NULL) {
        for(int i = 0; i < university->groupsCount; ++i) {
            if (strcmp(name, university->groups[i].name) == 0) {
                return &university->groups[i];
            }
        }
    }

    return NULL;
}

bool addNewGroup(University *university, const Group group) {

    if (university == NULL) {
        return false;
    }

    Group *groupPtr;

    if (university->groupsCount != 0) {
        groupPtr = (Group*)realloc(university->groups, (university->groupsCount + 1) * sizeof(Group));
    }
    else {
        groupPtr = (Group*)malloc(sizeof(Group));
    }

    if (groupPtr == NULL) {
        printf("memory allocation error\n");
        return false;
    }
    university->groups = groupPtr;
    university->groups[university->groupsCount] = group;
    university->groupsCount++;
    return true;
}

bool addNewStudent(Group *group, Student student) {

    if (group == NULL) {
        return false;
    }

    if (strcmp(student.groupName, group->name) != 0) {
        return false;
    }

    Student *studentPtr = NULL;

    if (group->studentsCount != 0) {
        studentPtr = (Student*)realloc(group->students, (group->studentsCount + 1) * sizeof(Student));
    }
    else {
        studentPtr = (Student*)malloc(sizeof(Student));
    }

    if (studentPtr == NULL) {
        printf("memory allocation error\n");
        return false;
    }

    group->students = studentPtr;

    if (student.id == 0) {
        student.id = g_Id;
    }
    group->students[group->studentsCount] = student;
    group->studentsCount++;

    for (int i = (int)(group->studentsCount - 1); i > 0; --i) {
        
        if (strcasecmp(group->students[i].surname, group->students[i - 1].surname) < 0) {
            Student tmp = group->students[i];
            group->students[i] = group->students[i - 1];
            group->students[i - 1] = tmp;
        } 
        else {
            break;
        }
        
    }

    if (student.id > g_Id) {
        g_Id = student.id + 1;
    }
    else {
        g_Id++;
    }

    return true;
}

bool removeGroup(University *university, const char *name) {

    if (university == NULL || name == NULL) {
        return false;
    }

    size_t sizeGr = sizeof(Group);

    for (int i = 0; i < university->groupsCount; ++i) {

        if (strcmp(name, university->groups[i].name) == 0) {
            free(university->groups[i].students);
            memmove(&university->groups[i], &university->groups[i + 1], (university->groupsCount - 1 - i) * sizeGr);
            university->groupsCount--;
            return true;
        }

    }

    return false;
}

bool removeStudent(University *university, const unsigned long id) {

    if (university == NULL) {
        return false;
    }

    size_t sizeSt = sizeof(Student);

    for (int i = 0; i < university->groupsCount; ++i) {

        for (int j = 0; j < university->groups[i].studentsCount; ++j) {

            if (id == university->groups[i].students[j].id) {

                memmove(&university->groups[i].students[j],
                        &university->groups[i].students[j + 1],
                        (university->groups[i].studentsCount - 1 - j) * sizeSt);
                university->groups[i].studentsCount--;
                return true;

            }
        }
    }
    return false;
}

University *initUniversity(const char *fileName) {

    University *university = (University*)malloc(sizeof(University));

    if (university == NULL) {
        printf("memory allocation error\n");
        return NULL;
    }

    university->groups = NULL;
    university->groupsCount = 0;

    FILE *file = fopen(fileName, "rb");
    if (file == NULL) {
        return university;
    }

    Student *student = NULL;
    Group *group = NULL;
    Group *groupCpy;
    size_t sizeSt = sizeof(Student);
    size_t sizeGr = sizeof(Group);

    while (!feof(file)) {

        student = (Student *) malloc(sizeSt);

        fread(student, sizeSt, 1, file);

        if (feof(file)) {
            free(student);
            break;
        }

        group = getGroup(university, student->groupName);

        if (group == NULL) {
            groupCpy = (Group *) malloc(sizeGr);

            strcpy(groupCpy->name, student->groupName);
            groupCpy->students = NULL;
            groupCpy->studentsCount = 0;

            if (!addNewGroup(university, *groupCpy)) {
                break;
            }
            free(groupCpy);

            group = &university->groups[university->groupsCount - 1];
        }

        if (!addNewStudent(group, *student)) {
            continue;
        }

        free(student);
    }

    fclose(file);
    
    int idCount = 0;
    
    for (int i = 0; i < university->groupsCount; i++) {
        for (int j = 0; j < university->groups[i].studentsCount; j++) {
            if (university->groups[i].students[j].id > idCount) {
                idCount = university->groups[i].students[j].id;
            }
        }
    }

    g_Id = idCount + 1;

    return university;
}

void printStudent(const Student student) {

    printf("\t%lu\n", student.id);
    printf("\t\t%s %s\n", student.surname, student.name);
    printf("\t\t%d\n", student.birthYear);

}

void printGroup(const Group group) {

    if (group.studentsCount == 0) {
        printf("empty group\n");
    }

    else {

        for (int i = 0; i < group.studentsCount; ++i) {
            printStudent(group.students[i]);
        }

    }

}

void printUniversity(const University *university) {

    if (university != NULL) {

        if (university->groupsCount == 0) {
            printf("University has no groups or students\n");
        }
        else {

            for (int i = 0; i < university->groupsCount; ++i) {
                printf("%s\n", university->groups[i].name);
                printGroup(university->groups[i]);
                printf("------------------------------------------------------------------------------\n");
            }

        }
    }
}

void freeUniversity(University *university) {

    if (university != NULL) {

        for (int i = 0; i < university->groupsCount; ++i) {
            free(university->groups[i].students);
        }

        free(university->groups);
        free(university);

    }
}

bool saveToFile(const char *fileName, const University *university) {

    FILE *file = fopen(fileName, "wb");

    if (file == NULL) {
        return false;
    }

    size_t sizeSt = sizeof(Student);

    for (int i = 0; i < university->groupsCount; ++i) {

        for (int j = 0; j < university->groups[i].studentsCount; ++j) {
            fwrite(&university->groups[i].students[j], sizeSt, 1, file);
        }

    }

    fclose(file);

    return true;
}
