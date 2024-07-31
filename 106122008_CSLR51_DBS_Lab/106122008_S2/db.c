#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000000
#define MAX_COURSES 4

typedef struct {
    int roll;
    char name[100];
    char dept[100];
    char courses[MAX_COURSES][100];
    int credits[MAX_COURSES];
    char grades[MAX_COURSES];
    int num_courses;
    double gpa;
} Student;

int gradePoints(char g) {
    switch (g) {
        case 'S': return 10;
        case 'A': return 9;
        case 'B': return 8;
        case 'C': return 7;
        case 'D': return 6;
        case 'E': return 5;
        default: return 0;
    }
}

double calculateGPA(Student *s) {
    double totalCredits = 0, totalPoints = 0;
    for (int i = 0; i < s->num_courses; i++) {
        totalCredits += s->credits[i];
        totalPoints += s->credits[i] * gradePoints(s->grades[i]);
    }
    return (totalCredits == 0) ? 0 : totalPoints / totalCredits;
}

void insertStudent() {
    FILE *file = fopen("students.txt", "a+");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    Student s;
    printf("Enter roll number: ");
    scanf("%d", &s.roll);
    printf("Enter name: ");
    scanf("%s", s.name);
    printf("Enter department: ");
    scanf("%s", s.dept);

    s.num_courses = MAX_COURSES;
    for (int i = 0; i < s.num_courses; i++) {
        printf("Enter course %d name: ", i + 1);
        scanf("%s", s.courses[i]);
        printf("Enter credits for course %d: ", i + 1);
        scanf("%d", &s.credits[i]);
        printf("Enter grade for course %d: ", i + 1);
        scanf(" %c", &s.grades[i]);
    }

    s.gpa = calculateGPA(&s);

    fprintf(file, "%d, %s, %s", s.roll, s.name, s.dept);
    for (int i = 0; i < s.num_courses; i++) {
        fprintf(file, ", %s, %d, %c", s.courses[i], s.credits[i], s.grades[i]);
    }
    fprintf(file, ", %.2f\n", s.gpa);

    fclose(file);
}

void createGPAColumn() {
    FILE *file = fopen("students.txt", "r");
    FILE *tempFile = fopen("tempfile.txt", "w");
    if (!file || !tempFile) {
        printf("Error opening file.\n");
        return;
    }

    char line[MAX];
    while (fgets(line, MAX, file)) {
        Student s;
        int num_fields = sscanf(line, "%d, %s, %s, %s, %d, %c, %s, %d, %c, %s, %d, %c, %lf",
                                &s.roll, s.name, s.dept,
                                s.courses[0], &s.credits[0], &s.grades[0],
                                s.courses[1], &s.credits[1], &s.grades[1],
                                s.courses[2], &s.credits[2], &s.grades[2],
                                &s.gpa);

        s.num_courses = (num_fields - 3) / 3; // Calculate number of courses
        s.gpa = calculateGPA(&s);

        fprintf(tempFile, "%d, %s, %s", s.roll, s.name, s.dept);
        for (int i = 0; i < s.num_courses; i++) {
            fprintf(tempFile, ", %s, %d, %c", s.courses[i], s.credits[i], s.grades[i]);
        }
        fprintf(tempFile, ", %.2f\n", s.gpa);
    }

    fclose(file);
    fclose(tempFile);

    remove("students.txt");
    rename("tempfile.txt", "students.txt");
}

void deregisterCourse(int rollno, const char *courseName) {
    FILE *file = fopen("students.txt", "r");
    FILE *tempFile = fopen("tempfile.txt", "w");
    if (!file || !tempFile) {
        printf("Error opening file.\n");
        return;
    }

    char line[MAX];

    Student s;
    while (fgets(line, MAX, file)) {
        int num_fields = sscanf(line, "%d, %s, %s, %s, %d, %c, %s, %d, %c, %s, %d, %c, %lf",
                                &s.roll, s.name, s.dept,
                                s.courses[0], &s.credits[0], &s.grades[0],
                                s.courses[1], &s.credits[1], &s.grades[1],
                                s.courses[2], &s.credits[2], &s.grades[2],
                                &s.gpa);

        if (s.roll == rollno) {
            int indexToRemove = -1;
            for (int i = 0; i < s.num_courses; i++) {
                if (strcmp(s.courses[i], courseName) == 0) {
                    indexToRemove = i;
                    break;
                }
            }

            if (indexToRemove != -1) {
                for (int i = indexToRemove; i < s.num_courses - 1; i++) {
                    strcpy(s.courses[i], s.courses[i + 1]);
                    s.credits[i] = s.credits[i + 1];
                    s.grades[i] = s.grades[i + 1];
                }
                s.num_courses--;
            }

            s.gpa = calculateGPA(&s);
        }

        fprintf(tempFile, "%d, %s, %s", s.roll, s.name, s.dept);
        for (int i = 0; i < s.num_courses; i++) {
            fprintf(tempFile, ", %s, %d, %c", s.courses[i], s.credits[i], s.grades[i]);
        }
        fprintf(tempFile, ", %.2f\n", s.gpa);
    }

    fclose(file);
    fclose(tempFile);

    remove("students.txt");
    rename("tempfile.txt", "students.txt");
}

void addCourse(int rollno, const char *courseName, int credits, char grade) {
    FILE *file = fopen("students.txt", "r");
    FILE *tempFile = fopen("tempfile.txt", "w");
    if (!file || !tempFile) {
        printf("Error opening file.\n");
        return;
    }

    char line[MAX];
    Student s;
    while (fgets(line, MAX, file)) {
        int num_fields = sscanf(line, "%d, %s, %s,  %s,  %d, %c, %s, %d, %c, %s, %d, %c, %lf",
                                &s.roll, s.name, s.dept,
                                s.courses[0], &s.credits[0], &s.grades[0],
                                s.courses[1], &s.credits[1], &s.grades[1],
                                s.courses[2], &s.credits[2], &s.grades[2],
                                &s.gpa);

        if (s.roll == rollno && s.num_courses < MAX_COURSES) {
            strcpy(s.courses[s.num_courses], courseName);
            s.credits[s.num_courses] = credits;
            s.grades[s.num_courses] = grade;
            s.num_courses++;
            s.gpa = calculateGPA(&s);
        }

        fprintf(tempFile, "%d, %s, %s", s.roll, s.name, s.dept);
        for (int i = 0; i < s.num_courses; i++) {
            fprintf(tempFile, ", %s, %d, %c", s.courses[i], s.credits[i], s.grades[i]);
        }
        fprintf(tempFile, ", %.2f\n", s.gpa);
    }

    fclose(file);
    fclose(tempFile);

    remove("students.txt");
    rename("tempfile.txt", "students.txt");
}

void updateCourseName(int rollno, const char *oldName, const char *newName) {
    FILE *file = fopen("students.txt", "r");
    FILE *tempFile = fopen("tempfile.txt", "w");
    if (!file || !tempFile) {
        printf("Error opening file.\n");
        return;
    }

    Student s;
    char line[MAX];
    while (fgets(line, MAX, file)) {
        int num_fields = sscanf(line, "%d, %s, %s, %s, %d, %c, %s, %d, %c, %s, %d, %c, %lf",
                                &s.roll, s.name, s.dept,
                                s.courses[0], &s.credits[0], &s.grades[0],
                                s.courses[1], &s.credits[1], &s.grades[1],
                                s.courses[2], &s.credits[2], &s.grades[2],
                                &s.gpa);

        if (s.roll == rollno) {
            for (int i = 0; i < s.num_courses; i++) {
                if (strcmp(s.courses[i], oldName) == 0) {
                    strcpy(s.courses[i], newName);
                    break;
                }
            }
            s.gpa = calculateGPA(&s);
        }

        fprintf(tempFile, "%d, %s, %s", s.roll, s.name, s.dept);
        for (int i = 0; i < s.num_courses; i++) {
            fprintf(tempFile, ", %s, %d, %c", s.courses[i], s.credits[i], s.grades[i]);
        }
        fprintf(tempFile, ", %.2f\n", s.gpa);
    }

    fclose(file);
    fclose(tempFile);

    remove("students.txt");
    rename("tempfile.txt", "students.txt");
}

void upgradeGrade(int rollno) {
    FILE *file = fopen("students.txt", "r");
    FILE *tempFile = fopen("tempfile.txt", "w");
    if (!file || !tempFile) {
        printf("Error opening file.\n");
        return;
    }

    char line[MAX];
    Student s;
    while (fgets(line, MAX, file)) {
        int num_fields = sscanf(line, "%d, %s, %s, %s, %d, %c, %s, %d, %c, %s, %d, %c, %lf",
                                &s.roll, s.name, s.dept,
                                s.courses[0], &s.credits[0], &s.grades[0],
                                s.courses[1], &s.credits[1], &s.grades[1],
                                s.courses[2], &s.credits[2], &s.grades[2],
                                &s.gpa);

        if (s.roll == rollno) {
            for (int i = 0; i < s.num_courses; i++) {
                if (s.grades[i] == 'C') {
                    s.grades[i] = 'B';  // Upgrade grade
                }
            }
            s.gpa = calculateGPA(&s);
        }

        fprintf(tempFile, "%d, %s, %s", s.roll, s.name, s.dept);
        for (int i = 0; i < s.num_courses; i++) {
            fprintf(tempFile, ", %s, %d, %c", s.courses[i], s.credits[i], s.grades[i]);
        }
        fprintf(tempFile, ", %.2f\n", s.gpa);
    }

    fclose(file);
    fclose(tempFile);

    remove("students.txt");
    rename("tempfile.txt", "students.txt");
}

void generateGradeReport(const char *identifier) {
    FILE *file = fopen("students.txt", "r");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    char line[MAX];
    Student s;
    int found = 0;

    while (fgets(line, MAX, file)) {
        int num_fields = sscanf(line, "%d, %s, %s, %s, %d, %c, %s, %d, %c, %s, %d, %c, %lf",
                                &s.roll, s.name, s.dept,
                                s.courses[0], &s.credits[0], &s.grades[0],
                                s.courses[1], &s.credits[1], &s.grades[1],
                                s.courses[2], &s.credits[2], &s.grades[2],
                                &s.gpa);

        if (strcmp(identifier, s.name) == 0 || strcmp(identifier, (char*) &s.roll) == 0) {
            printf("Roll Number: %d\n", s.roll);
            printf("Name: %s\n", s.name);
            printf("Department: %s\n", s.dept);
            for (int i = 0; i < s.num_courses; i++) {
                printf("Course %d: %s\n", i + 1, s.courses[i]);
                printf("Credits: %d\n", s.credits[i]);
                printf("Grade: %c\n", s.grades[i]);
            }
            printf("GPA: %.2f\n", s.gpa);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Student not found.\n");
    }

    fclose(file);
}

void displayMenu() {
    printf("1. Insert Student Record\n");
    printf("2. Create/Update GPA Column\n");
    printf("3. Deregister a Course\n");
    printf("4. Add a New Course\n");
    printf("5. Update Course Name\n");
    printf("6. Upgrade Grade\n");
    printf("7. Generate Grade Report\n");
    printf("8. Exit\n");
}

int main() {
    int choice;
    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                insertStudent();
                break;
            case 2:
                createGPAColumn();
                break;
            case 3: {
                int rollno;
                char courseName[100];
                printf("Enter roll number: ");
                scanf("%d", &rollno);
                printf("Enter course name to deregister: ");
                scanf("%s", courseName);
                deregisterCourse(rollno, courseName);
                break;
            }
            case 4: {
                int rollno, credits;
                char courseName[100], grade;
                printf("Enter roll number: ");
                scanf("%d", &rollno);
                printf("Enter new course name: ");
                scanf("%s", courseName);
                printf("Enter credits for new course: ");
                scanf("%d", &credits);
                printf("Enter grade for new course: ");
                scanf(" %c", &grade);
                addCourse(rollno, courseName, credits, grade);
                break;
            }
            case 5: {
                int rollno;
                char oldName[100], newName[100];
                printf("Enter roll number: ");
                scanf("%d", &rollno);
                printf("Enter old course name: ");
                scanf("%s", oldName);
                printf("Enter new course name: ");
                scanf("%s", newName);
                updateCourseName(rollno, oldName, newName);
                break;
            }
            case 6: {
                int rollno;
                printf("Enter roll number: ");
                scanf("%d", &rollno);
                upgradeGrade(rollno);
                break;
            }
            case 7: {
                char identifier[100];
                printf("Enter roll number or name: ");
                scanf("%s", identifier);
                generateGradeReport(identifier);
                break;
            }
            case 8:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n");
                break;
        }
    } while (choice != 8);

    return 0;
}
