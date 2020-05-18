#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// Utils
int findStudentById(vector<string> studentIds, string id);
int findLectureByCode(vector<string> lectureCodes, string code);
void deleteElement(vector<string>& v, int index);
void deleteElement(vector<int>& v, int index);

// File read
void readStudentFile(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes);
void readLectureFile(vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits);

// File write
void writeStudentFile(const vector<string>& studentIds, const vector<string>& passwords, const vector<string>& names, const vector<int>& credits, const vector<vector<string>>& appliedLectureCodes);
void writeLectureFile(const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, const vector<int>& limits);

// Get user input
string getInputId();
string getInputPassword();

// Login
int studentLogin(const vector<string>& studentIds, const vector<string>& passwords);
bool adminLogin();
int login(const vector<string>& studentIds, const vector<string>& passwords);

// Common
void printLectures(const vector<vector<string>>& appliedLectureCodes, const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, const vector<int>& limits, const int& user = -100);

// Admin
void addStudent(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes);
void addLecture(vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits);
void deleteLecture(vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits, vector<int>& credits, vector<vector<string>>& appliedLectureCodes);
int runAdmin(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits);

// User
void printStudent(const vector<string>& studentIds, const vector<string>& names, const vector<int>& credits, const int& user);
void applyLecture(const vector<string>& studentIds, const vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, vector<int>& limits, const int& user);
void disapplyLecture(const vector<string>& studentIds, const vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, vector<int>& limits, const int& user);
void changePassword(vector<string>& passwords, const int& user);
int runStudent(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits, int user);

int main() {
	int user = -1; //user index
	int status = -1;

	// Student Info
	vector<string> studentIds;
	vector<string> passwords;
	vector<string> names;
	vector<int> credits;
	vector<vector<string>> appliedLectureCodes;

	// Lecture Info
	vector<string> lectureCodes;
	vector<string> lectureNames;
	vector<int> lectureCredits;
	vector<int> limits;

	// Read from files
	readStudentFile(studentIds, passwords, names, credits, appliedLectureCodes);
	readLectureFile(lectureCodes, lectureNames, lectureCredits, limits);

	// Login phase
	while (status == -1) {
		user = login(studentIds, passwords);

		if (user == -999) { // Login fail
			break;
		}
		else if (user == -1) { // Exit command
			break;
		}
		else if (user == -100) { // Admin login success
			status = runAdmin(studentIds, passwords, names, credits, appliedLectureCodes, lectureCodes, lectureNames, lectureCredits, limits);
		}
		else { // Student login success
			status = runStudent(studentIds, passwords, names, credits, appliedLectureCodes, lectureCodes, lectureNames, lectureCredits, limits, user);
		}
	}

	// Write to files
	writeStudentFile(studentIds, passwords, names, credits, appliedLectureCodes);
	writeLectureFile(lectureCodes, lectureNames, lectureCredits, limits);

	return 0;
}

int findStudentById(vector<string> studentIds, string id) {
	for (unsigned i = 0; i < studentIds.size(); i++) {
		if (studentIds[i] == id)
			return i;
	}
	return -1;
}

int findLectureByCode(vector<string> lectureCodes, string code) {
	for (unsigned i = 0; i < lectureCodes.size(); i++) {
		if (lectureCodes[i] == code)
			return i;
	}
	return -1;
}

void deleteElement(vector<string>& v, int index) {
	v.erase(v.begin() + index);
}

void deleteElement(vector<int>& v, int index) {
	v.erase(v.begin() + index);
}

void readStudentFile(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes) {
	ifstream fin("students.txt");
	if (!fin)
		cout << "file open error!!" << endl;
	string field;
	int credits_field;
	while (1) {
		fin >> field;
		if (!fin)
			break;
		studentIds.push_back(field);
		fin >> field;
		passwords.push_back(field);
		fin >> field;
		names.push_back(field);
		fin >> credits_field;
		credits.push_back(credits_field);
		int lengthOfappliedLecture;
		fin >> lengthOfappliedLecture;

		vector<string> codes;
		for (int j = 0; j < lengthOfappliedLecture; j++) {
			fin >> field;
			codes.push_back(field);
		}
		appliedLectureCodes.push_back(codes);
	}
	fin.close();
}


void readLectureFile(vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits) {
	ifstream fin("lectures.txt");
	while (true) {
		string str_field;
		int int_field;
		fin >> str_field;
		if (!fin)
			break;
		lectureCodes.push_back(str_field);
		fin >> str_field;
		lectureNames.push_back(str_field);
		fin >> int_field;
		lectureCredits.push_back(int_field);
		fin >> int_field;
		limits.push_back(int_field);
	}
	fin.close();
}

void writeStudentFile(const vector<string>& studentIds, const vector<string>& passwords, const vector<string>& names, const vector<int>& credits, const vector<vector<string>>& appliedLectureCodes) {
	ofstream fout("students.txt");
	for (int i = 0; i < studentIds.size(); i++) {
		fout << studentIds[i] << "\t" << passwords[i] << "\t" << names[i] << "\t" << credits[i] << "\t" << appliedLectureCodes[i].size() << "\t";
		for (int j = 0; j < appliedLectureCodes[i].size(); j++) {
			fout << appliedLectureCodes[i][j] << " ";
		}
		fout << endl;
	}
	fout.close();
}

void writeLectureFile(const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, const vector<int>& limits) {
	ofstream fout("lectures.txt");
	for (int i = 0; i < lectureCodes.size(); i++) {
		fout << lectureCodes[i] << "\t" << lectureNames[i] << "\t" << lectureCredits[i] << "\t" << limits[i] << endl;
	}
	fout.close();
}

string getInputId() {
	string id;
	cout << "���̵�: ";
	cin >> id;
	return id;
	//����� �Է� ���̵� ��ȯ
}

string getInputPassword() {
	string password;
	cout << "��й�ȣ: ";
	cin >> password;
	return password;
	//����� �Է� ��й�ȣ ��ȯ
}

int studentLogin(const vector<string>& studentIds, const vector<string>& passwords) {
	cout << "--------------------------------------------------------------------------" << endl;
	string inputId = getInputId();
	string inputPassword = getInputPassword();
	cout << "--------------------------------------------------------------------------" << endl;

	for (int i = 0; i < studentIds.size(); i++) {
		if (inputId == studentIds[i]) {
			if (inputPassword == passwords[i])
				return i;
		}
	}
	return -1;
	/* ���� ȣ�� �Լ�: getInputId(), getInputPassword()*/
	//����(���̵� ���� ���� �ε���), ����(-1)
}

bool adminLogin() {
	string inputId = getInputId();
	string inputPassword = getInputPassword();

	if (inputId == "admin") {
		if (inputPassword == "admin")
			return true;
	}

	return false;
	/* ���� ȣ�� �Լ�: getInputId(), getInputPassword()*/
	//����(true) ���� (false)
}

int login(const vector<string>& studentIds, const vector<string>& passwords) {
	int choice;
	cout << "--------------------------------------------------------------------------" << endl;
	cout << "1. �л� �α���" << endl;
	cout << "2. ������ �α���" << endl;
	cout << "0. ����" << endl;
	cout << "--------------------------------------------------------------------------" << endl;
	cout << ">> ";
	cin >> choice;
	int result = 0;
	switch (choice)
	{
	case 1:
		for (int i = 0; i < 3; i++) {
			result = studentLogin(studentIds, passwords);
			if (result == -1)
				cout << "�α���" << i + 1 << "ȸ ����(3ȸ ���� �� ����)" << endl;
			else
				return result;
		}
		cout << "3ȸ �����Ͽ� �����մϴ�" << endl;
		return -999;
		break;
	case 2:
		for (int i = 0; i < 3; i++) {
			result = adminLogin();
			if (result == false)
				cout << "�α���" << i + 1 << "ȸ ����(3ȸ ���� �� ����)" << endl;
			else
				return -100;
		}
		cout << "3ȸ �����Ͽ� �����մϴ�" << endl;
		return -999;
		break;
	case 0:
		return -1;
		break;
	}
	/* ���� ȣ�� �Լ�: studentLogin(), adminLogin()*/
	//��ü���� �α��� ������ ���� �л�����(���̵� ���� ���� �ε���), ������ ����(-100). ����(-999)��ȯ
}

void printLectures(const vector<vector<string>>& appliedLectureCodes, const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, const vector<int>& limits, const int& user) {
	cout << "--------------------------------------------------------------------------" << endl;
	cout << "�����ڵ�\t���Ǹ�\t\t����\t���������ο�" << endl;
	cout << "--------------------------------------------------------------------------" << endl;

	if (user == -100) { //�������� ��� ���
		for (int i = 0; i < lectureCodes.size(); i++)
			cout << lectureCodes[i] << "\t" << lectureNames[i] << "\t" << lectureCredits[i] << "\t" << limits[i] << endl;
	}
	else {
		for (int i = 0; i < appliedLectureCodes[user].size(); i++) {
			int index = findLectureByCode(lectureCodes, appliedLectureCodes[user][i]);
			cout << lectureCodes[index] << "\t" << lectureNames[index] << "\t" << lectureCredits[index] << "\t" << limits[index] << endl;
		}
	}
}

void addStudent(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes) {
	string inputId, inputPassword, inputName;

	cout << "�й�: ";
	cin >> inputId;
	cout << "��й�ȣ: ";
	cin >> inputPassword;
	cout << "�л� �̸�: ";
	cin >> inputName;

	int flag = findStudentById(studentIds, inputId);
	if (flag == -1) {
		studentIds.push_back(inputId);
		passwords.push_back(inputPassword);
		names.push_back(inputName);
		credits.push_back(18);
		int size = appliedLectureCodes.size();
		vector<string> Vector;
		appliedLectureCodes.push_back(Vector);
		cout << "���������� ��ϵǾ����ϴ�." << endl;
	}
	else
		cout << "�̹� �����ϴ� �й��Դϴ�." << endl;

	system("pause");

	//�л��߰�
}

void addLecture(vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits) {
	string inputCode, inputName;
	int inputCredit, inputLimit;
	cout << "--------------------------------------------------------------------------" << endl;
	cout << "�����ڵ�: ";
	cin >> inputCode;
	cout << "�����: ";
	cin >> inputName;
	cout << "����: ";
	cin >> inputCredit;
	cout << "�����ο�: ";
	cin >> inputLimit;
	cout << endl;
	cout << "--------------------------------------------------------------------------" << endl;

	int indexOfInputCode = findLectureByCode(lectureCodes, inputCode);
	if (indexOfInputCode == -1) {
		lectureCodes.push_back(inputCode);
		lectureNames.push_back(inputName);
		lectureCredits.push_back(inputCredit);
		limits.push_back(inputLimit);
		cout << "���������� ���ǰ� �����Ǿ����ϴ�." << endl;
	}
	else
		cout << "�̹� �����ϴ� �����ڵ� �Դϴ�." << endl;

	system("pause");
}

void deleteLecture(vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits, vector<int>& credits, vector<vector<string>>& appliedLectureCodes) {
	string lectureCodeToDelete;
	while (true) {
		printLectures(appliedLectureCodes, lectureCodes, lectureNames, lectureCredits, limits);

		cout << "������ ���� �ڵ�(0: �ڷΰ���) >> ";
		cin >> lectureCodeToDelete;
		if (lectureCodeToDelete == "0")
			return;

		int lectureIndexToDelete = findLectureByCode(lectureCodes, lectureCodeToDelete);
		if (lectureIndexToDelete == -1)
			cout << "��ġ�ϴ� �ڵ尡 �����ϴ�." << endl;
		else {
			for (int user = 0; user < appliedLectureCodes.size(); user++) {
				int indexOfAppliedLectures = findLectureByCode(appliedLectureCodes[user], lectureCodeToDelete);
				if (indexOfAppliedLectures != -1) {//������ ���Ǹ� ��û�� �л��� ������ 
					deleteElement(appliedLectureCodes[user], indexOfAppliedLectures); //����öȸ
					credits[user] += lectureCredits[lectureIndexToDelete];
				}
			}
			deleteElement(lectureCodes, lectureIndexToDelete);
			deleteElement(lectureNames, lectureIndexToDelete);
			deleteElement(lectureCredits, lectureIndexToDelete);
			deleteElement(limits, lectureIndexToDelete);
			cout << "���������� ���ǰ� ���Ǿ����ϴ�." << endl;

			/*vector<int> updatedLectureCredits, updatedLimits;
			for (int i = 0; i < lectureCredits.size(); i++) {
			if (i == lectureIndexToDelete)
			continue;
			updatedLectureCredits.push_back(lectureCredits[i]);
			updatedLimits.push_back(limits[i]);
			}
			lectureCredits = updatedLectureCredits;
			limits = updatedLimits;
			cout << "���������� ���ǰ� ���Ǿ����ϴ�." << endl;
			}*/
			system("pause");
		}
		/* ���� ȣ�� �Լ�: printLectures*/
	}
}

int runAdmin(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits) {
	while (true) {
		cout << "--------------------------------------------------------------------------" << endl;
		cout << "1. �л� �߰�" << endl;
		cout << "2. ���� ����" << endl;
		cout << "3. ���� ����" << endl;
		cout << "4. �α׾ƿ�" << endl;
		cout << "0. ����" << endl;
		cout << "--------------------------------------------------------------------------" << endl;

		int choice;
		cout << ">> ";
		cin >> choice;

		switch (choice)
		{
		case 1:
			addStudent(studentIds, passwords, names, credits, appliedLectureCodes);
			break;
		case 2:
			addLecture(lectureCodes, lectureNames, lectureCredits, limits);
			break;
		case 3:
			deleteLecture(lectureCodes, lectureNames, lectureCredits, limits, credits, appliedLectureCodes);
			break;
		case 4:
			return -1;
			break;
		case 0:
			return 1;
			break;

		}
	}
	/* ���� ȣ�� �Լ�: addStudent, addLecture, deleteLecture*/
}

void printStudent(const vector<string>& studentIds, const vector<string>& names, const vector<int>& credits, const int& user) {
	cout << "--------------------------------------------------------------------------" << endl;
	cout << "�й�: " << studentIds[user] << "\t�̸�: " << names[user] << "\t������������: " << credits[user] << endl;
	cout << "--------------------------------------------------------------------------" << endl;
	//������ �л� ���� �й�, �̸�, ������������, �л��޴����� ������û, ������ȯ ����öȸ �޴����� ȣ��
}

void applyLecture(const vector<string>& studentIds, const vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, vector<int>& limits, const int& user) {
	string inputLectureCode;

	while (true) {
		cout << "--------------------------------------------------------------------------" << endl;
		printStudent(studentIds, names, credits, user);
		printLectures(appliedLectureCodes, lectureCodes, lectureNames, lectureCredits, limits);
		cout << "--------------------------------------------------------------------------" << endl;
		cout << "��û�� ���� �ڵ�(0: �ڷΰ���) >> ";
		cin >> inputLectureCode;

		if (inputLectureCode == "0")
			return;

		int inputLectureIndex = findLectureByCode(lectureCodes, inputLectureCode);

		bool lectureNameReduplication = false;
		for (int i = 0; i < lectureCodes.size(); i++) {
			if (lectureNames[inputLectureIndex] == lectureNames[i]) {
				if (findLectureByCode(appliedLectureCodes[user], lectureCodes[i]) != -1) {
					lectureNameReduplication = true;
					break;
				}
			}
		}

		if (limits[inputLectureIndex] == 0)
			cout << "�����ο��� �� á���ϴ�." << endl;

		else if (credits[user] - lectureCredits[inputLectureIndex] < 0)
			cout << "�������������� �����մϴ�." << endl;

		else if (findLectureByCode(appliedLectureCodes[user], inputLectureCode) != -1)
			cout << "�̹� �ش� ������ ��û�߽��ϴ�." << endl;

		else if (lectureNameReduplication) {
			cout << "�̹� ���ϸ��� ������ ��û�߽��ϴ�." << endl;
		}

		else {
			credits[user] -= lectureCredits[inputLectureIndex];
			appliedLectureCodes[user].push_back(inputLectureCode);
			limits[inputLectureIndex] -= 1;
			cout << "[" << inputLectureCode << "] " << lectureNames[inputLectureIndex] << "(��)�� ���������� ��û�Ͽ����ϴ�." << endl;
		}

		system("pause");
	}
	/* ���� ȣ�� �Լ�: printStudent, printLectures*/
}

void disapplyLecture(const vector<string>& studentIds, const vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, vector<int>& limits, const int& user) {
	while (true) {
		printStudent(studentIds, names, credits, user);
		printLectures(appliedLectureCodes, lectureCodes, lectureNames, lectureCredits, limits, user);

		string LectureCodeToDelete;
		cout << "öȸ�� ���� �ڵ�(0: �ڷΰ���) >>";
		cin >> LectureCodeToDelete;

		if (LectureCodeToDelete == "0")
			return;

		int lectureIndexToDelete = findLectureByCode(lectureCodes, LectureCodeToDelete);//������ ���� �ڵ��� �ε���
		int appliedIndex = findLectureByCode(appliedLectureCodes[user], LectureCodeToDelete); //��û�� �����߿� ������ �ڵ��� �ε���
		if (appliedIndex != -1) {
			deleteElement(appliedLectureCodes[user], appliedIndex);
			credits[user] += lectureCredits[lectureIndexToDelete];
			limits[lectureIndexToDelete]++;
			cout << "[" << LectureCodeToDelete << "] " << lectureNames[lectureIndexToDelete] << "��(��) öȸ�Ͽ����ϴ�." << endl;
		}
		else
			cout << "���� �ڵ尡 �ùٸ��� �ʽ��ϴ�." << endl;
		system("pause");
	}

	/* ���� ȣ�� �Լ�: printStudent, printLectures*/
}

void changePassword(vector<string>& passwords, const int& user) {
	string inputPassword;
	string newPassword;
	cout << "--------------------------------------------------------------------------" << endl;
	cout << "���� Ȯ���� ���� ��й�ȣ�� �� �� �� �Է����ּ���." << endl;
	cin >> inputPassword;
	cout << "--------------------------------------------------------------------------" << endl;

	if (inputPassword == passwords[user]) {
		cout << "--------------------------------------------------------------------------" << endl;
		cout << "���� ������ ��й�ȣ�� �Է��ϼ���." << endl;
		cout << "��й�ȣ: ";
		cin >> newPassword;
		passwords[user] = newPassword;
		cout << "����Ǿ����ϴ�." << endl;
		cout << "--------------------------------------------------------------------------" << endl;
	}
	else
		cout << "��й�ȣ�� ��ġ���� �ʽ��ϴ�." << endl;
	system("pause");
}

int runStudent(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits, int user) {
	int choice;

	while (true) {
		cout << "--------------------------------------------------------------------------" << endl;
		cout << "1. ���� ��û" << endl;
		cout << "2. ���� ��Ȳ" << endl;
		cout << "3. ���� öȸ" << endl;
		cout << "4. ��й�ȣ ����" << endl;
		cout << "5. �α׾ƿ�" << endl;
		cout << "0. ����" << endl;
		cout << "--------------------------------------------------------------------------" << endl;
		cout << ">> ";
		cin >> choice;

		switch (choice)
		{
		case 1:
			applyLecture(studentIds, names, credits, appliedLectureCodes, lectureCodes, lectureNames, lectureCredits, limits, user);
			break;
		case 2:
			printStudent(studentIds, names, credits, user);
			printLectures(appliedLectureCodes, lectureCodes, lectureNames, lectureCredits, limits, user);
			system("pause");
			break;
		case 3:
			disapplyLecture(studentIds, names, credits, appliedLectureCodes, lectureCodes, lectureNames, lectureCredits, limits, user);
			break;
		case 4:
			changePassword(passwords, user);
			break;
		case 5:
			return -1;
			break;
		case 0:
			return 1;
			break;
		}
	}
	/* ���� ȣ�� �Լ�: applyLecture, printStudent, printLectures, disapplyLecture, changePassword*/
	// �л� �޴� �Ѱ� �α׾ƿ�(-1) ����(1) ��ȯ
	// 
}
