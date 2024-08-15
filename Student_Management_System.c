#include<stdio.h>
#include <stdlib.h>

struct student {
	int id;
	char name[50];
	int age;
	float gpa;
};

typedef struct {
	struct student data;
	struct node *next;
} node;
node *head = NULL;
node *tail = NULL;

void addStudent(const struct student *const ptr);
void displayStudents(void);
void searchStudentByID(int id);
void updateStudent(int id);
void deleteStudent(int id);
float calculateAverageGPA(void);
void searchHighestGPA(void);
void choice(void);
void PrintStudent(const struct student *const current);
void IterateLinkedlist(int id, node **current);

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	printf("Welcome \n");
	choice();
	return 0;
}

void choice(void) {
	struct student s1;
	int num, id;
	float AVG;
	printf("--------------------------------------------\n");
	printf("Enter 1 for Add a Student: \n");
	printf("Enter 2 for Display All Students: \n");
	printf("Enter 3 for Search for a Student by ID: \n");
	printf("Enter 4 for Update Student Information: \n");
	printf("Enter 5 for Delete a Student: \n");
	printf("Enter 6 for Calculate Average GPA: \n");
	printf("Enter 7 for Find Student with Highest GPA: \n");
	printf("Enter 8 for Exit: \n");
	printf("Enter you choice: ");
	scanf("%d", &num);
	printf("--------------------------------------------\n");
	if (num == 1 || num == 3 || num == 4||num ==5) {
		printf("Enter id : ");
		scanf("%d", &id);
	}
	switch (num) {
	case 1:
		s1.id = id;
		printf("Enter GPA : ");
		scanf("%f", &s1.gpa);
		printf("Enter age: ");
		scanf("%d", &s1.age);
		getchar();
		printf("Enter name: ");
		scanf("%[^\n]", s1.name);
		printf("--------------------------------------------\n");
		addStudent(&s1);
		break;
	case 2:
		displayStudents();
		break;
	case 3:
		searchStudentByID(id);
		break;
	case 4:
		updateStudent(id);
		break;
	case 5:
		deleteStudent(id);
		break;
	case 6:
		AVG= calculateAverageGPA();
		printf("Average GPA is: %.3f\n",AVG);
		break;
	case 7:
		searchHighestGPA();
		break;
	case 8:
		return;
	}
	choice();
}

void addStudent(const struct student *const ptr) {
	node *nptr = head;
	while (nptr != NULL) {
		if (ptr->id == nptr->data.id) {
			printf("The student with this ID is existed.\n");
			return;
		}
		nptr = nptr->next;
	}
	node *current = (node*) malloc(sizeof(node));
	if (current == NULL) {
		printf("Error!\n");
		return;
	}
	current->data.id = ptr->id;
	current->data.age = ptr->age;
	current->data.gpa = ptr->gpa;
	strcpy(current->data.name, ptr->name);
	current->next =NULL;
	if (head == NULL) {
		head = current;
		tail = current;
	} else {
		tail->next = current;
		tail = current;
	}
}

void displayStudents(void) {
	if (head == NULL) {
		printf("There are no students.\n");
		return;
	} else {
		node *current = head;
		while (current != NULL) {
			PrintStudent(&current->data);
			current = current->next;
		}
	}
}

void searchStudentByID(int id) {
	node *key = NULL;
	IterateLinkedlist(id, &key);
	if (key != NULL) {
		PrintStudent(&key->data);
	}
}

void updateStudent(int id) {
	node *key = NULL;
	IterateLinkedlist(id, &key);
	if (key != NULL) {
		printf("Enter new GPA : ");
		scanf("%f", &key->data.gpa);
		printf("Enter new age: ");
		scanf("%d", &key->data.age);
		getchar();
		printf("Enter new name: ");
		gets(key->data.name);
		printf("student updated successfully\n");
	}
}

void deleteStudent(int id) {
	if (head != NULL) {
		node *current = head;
		if (head->data.id == id) {
			head = head->next;
			free(current);
			printf("student deleted successfully\n");
		} else {
			node *pre = head;
			current = head->next;
			while ((current != NULL)) {
				if (id == current->data.id) {
					pre->next = current->next;
					free(current);
					printf("student deleted successfully\n");
					return;
				} else {
					pre = current;
					current = current->next;
				}
			}
			printf("The student with this ID isn't existed.\n");
		}
	}
}

float calculateAverageGPA(void) {
	int count = 0;
	float sum = 0;
	if (head != NULL) {
		node *current = head;
		while ((current != NULL)) {
			count++;
			sum += current->data.gpa;
			current = current->next;
		}
		return ( sum / count);
	}
	return 0;
}

void searchHighestGPA(void) {
	if (head != NULL) {
		node *current = head;
		node *max = head;
		while ((current != NULL)) {
			if (max->data.gpa < current->data.gpa) {
				max->data.gpa = current->data.gpa;
			}
			current = current->next;
		}
		PrintStudent(&max->data);
	} else {
		printf("There are no students.\n");
	}
}

void PrintStudent(const struct student *const current) {
	printf("[ ID:%d ,",current->id);
	printf("Name:%s ,", current->name);
	printf("Age:%d ,", current->age);
	printf("GPA:%.2f ]\n", current->gpa);
}

void IterateLinkedlist(int id, node **current) {
	if (head == NULL) {
		printf("There are no students.\n");
		return;
	} else {
		*current = head;
		while ((*current) != NULL) {
			if (id == (*current)->data.id) {
				return;
			} else {
				*current = (*current)->next;
			}
		}
		printf("The student with this ID isn't existed.\n");
		*current = NULL;
	}
}

