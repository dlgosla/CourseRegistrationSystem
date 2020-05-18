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
	cout << "아이디: ";
	cin >> id;
	return id;
	//사용자 입력 아이디 반환
}

string getInputPassword() {
	string password;
	cout << "비밀번호: ";
	cin >> password;
	return password;
	//사용자 입력 비밀번호 반환
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
	/* 내부 호출 함수: getInputId(), getInputPassword()*/
	//성공(아이디 벡터 내의 인덱스), 실패(-1)
}

bool adminLogin() {
	string inputId = getInputId();
	string inputPassword = getInputPassword();

	if (inputId == "admin") {
		if (inputPassword == "admin")
			return true;
	}

	return false;
	/* 내부 호출 함수: getInputId(), getInputPassword()*/
	//성공(true) 실패 (false)
}

int login(const vector<string>& studentIds, const vector<string>& passwords) {
	int choice;
	cout << "--------------------------------------------------------------------------" << endl;
	cout << "1. 학생 로그인" << endl;
	cout << "2. 관리자 로그인" << endl;
	cout << "0. 종료" << endl;
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
				cout << "로그인" << i + 1 << "회 실패(3회 실패 시 종료)" << endl;
			else
				return result;
		}
		cout << "3회 실패하여 종료합니다" << endl;
		return -999;
		break;
	case 2:
		for (int i = 0; i < 3; i++) {
			result = adminLogin();
			if (result == false)
				cout << "로그인" << i + 1 << "회 실패(3회 실패 시 종료)" << endl;
			else
				return -100;
		}
		cout << "3회 실패하여 종료합니다" << endl;
		return -999;
		break;
	case 0:
		return -1;
		break;
	}
	/* 내부 호출 함수: studentLogin(), adminLogin()*/
	//전체적인 로그인 과정을 관리 학생성공(아이디 벡터 내의 인덱스), 관리자 성공(-100). 실패(-999)반환
}

void printLectures(const vector<vector<string>>& appliedLectureCodes, const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, const vector<int>& limits, const int& user) {
	cout << "--------------------------------------------------------------------------" << endl;
	cout << "과목코드\t강의명\t\t학점\t수강가능인원" << endl;
	cout << "--------------------------------------------------------------------------" << endl;

	if (user == -100) { //개설강의 목록 출력
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

	cout << "학번: ";
	cin >> inputId;
	cout << "비밀번호: ";
	cin >> inputPassword;
	cout << "학생 이름: ";
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
		cout << "성공적으로 등록되었습니다." << endl;
	}
	else
		cout << "이미 존재하는 학번입니다." << endl;

	system("pause");

	//학생추가
}

void addLecture(vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits) {
	string inputCode, inputName;
	int inputCredit, inputLimit;
	cout << "--------------------------------------------------------------------------" << endl;
	cout << "과목코드: ";
	cin >> inputCode;
	cout << "과목명: ";
	cin >> inputName;
	cout << "학점: ";
	cin >> inputCredit;
	cout << "수강인원: ";
	cin >> inputLimit;
	cout << endl;
	cout << "--------------------------------------------------------------------------" << endl;

	int indexOfInputCode = findLectureByCode(lectureCodes, inputCode);
	if (indexOfInputCode == -1) {
		lectureCodes.push_back(inputCode);
		lectureNames.push_back(inputName);
		lectureCredits.push_back(inputCredit);
		limits.push_back(inputLimit);
		cout << "성공적으로 강의가 개설되었습니다." << endl;
	}
	else
		cout << "이미 존재하는 과목코드 입니다." << endl;

	system("pause");
}

void deleteLecture(vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits, vector<int>& credits, vector<vector<string>>& appliedLectureCodes) {
	string lectureCodeToDelete;
	while (true) {
		printLectures(appliedLectureCodes, lectureCodes, lectureNames, lectureCredits, limits);

		cout << "삭제할 강의 코드(0: 뒤로가기) >> ";
		cin >> lectureCodeToDelete;
		if (lectureCodeToDelete == "0")
			return;

		int lectureIndexToDelete = findLectureByCode(lectureCodes, lectureCodeToDelete);
		if (lectureIndexToDelete == -1)
			cout << "일치하는 코드가 없습니다." << endl;
		else {
			for (int user = 0; user < appliedLectureCodes.size(); user++) {
				int indexOfAppliedLectures = findLectureByCode(appliedLectureCodes[user], lectureCodeToDelete);
				if (indexOfAppliedLectures != -1) {//삭제할 강의를 신청한 학생이 있으면 
					deleteElement(appliedLectureCodes[user], indexOfAppliedLectures); //수강철회
					credits[user] += lectureCredits[lectureIndexToDelete];
				}
			}
			deleteElement(lectureCodes, lectureIndexToDelete);
			deleteElement(lectureNames, lectureIndexToDelete);
			deleteElement(lectureCredits, lectureIndexToDelete);
			deleteElement(limits, lectureIndexToDelete);
			cout << "성공적으로 강의가 폐쇄되었습니다." << endl;

			/*vector<int> updatedLectureCredits, updatedLimits;
			for (int i = 0; i < lectureCredits.size(); i++) {
			if (i == lectureIndexToDelete)
			continue;
			updatedLectureCredits.push_back(lectureCredits[i]);
			updatedLimits.push_back(limits[i]);
			}
			lectureCredits = updatedLectureCredits;
			limits = updatedLimits;
			cout << "성공적으로 강의가 폐쇄되었습니다." << endl;
			}*/
			system("pause");
		}
		/* 내부 호출 함수: printLectures*/
	}
}

int runAdmin(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits) {
	while (true) {
		cout << "--------------------------------------------------------------------------" << endl;
		cout << "1. 학생 추가" << endl;
		cout << "2. 강의 개설" << endl;
		cout << "3. 강의 삭제" << endl;
		cout << "4. 로그아웃" << endl;
		cout << "0. 종료" << endl;
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
	/* 내부 호출 함수: addStudent, addLecture, deleteLecture*/
}

void printStudent(const vector<string>& studentIds, const vector<string>& names, const vector<int>& credits, const int& user) {
	cout << "--------------------------------------------------------------------------" << endl;
	cout << "학번: " << studentIds[user] << "\t이름: " << names[user] << "\t수강가능학점: " << credits[user] << endl;
	cout << "--------------------------------------------------------------------------" << endl;
	//간단한 학생 정보 학번, 이름, 수강가능학점, 학생메뉴에서 수강신청, 수강현환 수강철회 메뉴에서 호출
}

void applyLecture(const vector<string>& studentIds, const vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, vector<int>& limits, const int& user) {
	string inputLectureCode;

	while (true) {
		cout << "--------------------------------------------------------------------------" << endl;
		printStudent(studentIds, names, credits, user);
		printLectures(appliedLectureCodes, lectureCodes, lectureNames, lectureCredits, limits);
		cout << "--------------------------------------------------------------------------" << endl;
		cout << "신청할 과목 코드(0: 뒤로가기) >> ";
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
			cout << "수강인원이 꽉 찼습니다." << endl;

		else if (credits[user] - lectureCredits[inputLectureIndex] < 0)
			cout << "수강가능학점이 부족합니다." << endl;

		else if (findLectureByCode(appliedLectureCodes[user], inputLectureCode) != -1)
			cout << "이미 해당 과목을 신청했습니다." << endl;

		else if (lectureNameReduplication) {
			cout << "이미 동일명의 과목을 신청했습니다." << endl;
		}

		else {
			credits[user] -= lectureCredits[inputLectureIndex];
			appliedLectureCodes[user].push_back(inputLectureCode);
			limits[inputLectureIndex] -= 1;
			cout << "[" << inputLectureCode << "] " << lectureNames[inputLectureIndex] << "(을)를 성공적으로 신청하였습니다." << endl;
		}

		system("pause");
	}
	/* 내부 호출 함수: printStudent, printLectures*/
}

void disapplyLecture(const vector<string>& studentIds, const vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, const vector<string>& lectureCodes, const vector<string>& lectureNames, const vector<int>& lectureCredits, vector<int>& limits, const int& user) {
	while (true) {
		printStudent(studentIds, names, credits, user);
		printLectures(appliedLectureCodes, lectureCodes, lectureNames, lectureCredits, limits, user);

		string LectureCodeToDelete;
		cout << "철회할 과목 코드(0: 뒤로가기) >>";
		cin >> LectureCodeToDelete;

		if (LectureCodeToDelete == "0")
			return;

		int lectureIndexToDelete = findLectureByCode(lectureCodes, LectureCodeToDelete);//삭제할 강의 코드의 인덱스
		int appliedIndex = findLectureByCode(appliedLectureCodes[user], LectureCodeToDelete); //신청한 과목중에 삭제할 코드의 인덱스
		if (appliedIndex != -1) {
			deleteElement(appliedLectureCodes[user], appliedIndex);
			credits[user] += lectureCredits[lectureIndexToDelete];
			limits[lectureIndexToDelete]++;
			cout << "[" << LectureCodeToDelete << "] " << lectureNames[lectureIndexToDelete] << "을(를) 철회하였습니다." << endl;
		}
		else
			cout << "과목 코드가 올바르지 않습니다." << endl;
		system("pause");
	}

	/* 내부 호출 함수: printStudent, printLectures*/
}

void changePassword(vector<string>& passwords, const int& user) {
	string inputPassword;
	string newPassword;
	cout << "--------------------------------------------------------------------------" << endl;
	cout << "본인 확인을 위해 비밀번호를 한 번 더 입력해주세요." << endl;
	cin >> inputPassword;
	cout << "--------------------------------------------------------------------------" << endl;

	if (inputPassword == passwords[user]) {
		cout << "--------------------------------------------------------------------------" << endl;
		cout << "새로 설정할 비밀번호를 입력하세요." << endl;
		cout << "비밀번호: ";
		cin >> newPassword;
		passwords[user] = newPassword;
		cout << "변경되었습니다." << endl;
		cout << "--------------------------------------------------------------------------" << endl;
	}
	else
		cout << "비밀번호가 일치하지 않습니다." << endl;
	system("pause");
}

int runStudent(vector<string>& studentIds, vector<string>& passwords, vector<string>& names, vector<int>& credits, vector<vector<string>>& appliedLectureCodes, vector<string>& lectureCodes, vector<string>& lectureNames, vector<int>& lectureCredits, vector<int>& limits, int user) {
	int choice;

	while (true) {
		cout << "--------------------------------------------------------------------------" << endl;
		cout << "1. 수강 신청" << endl;
		cout << "2. 수강 현황" << endl;
		cout << "3. 수강 철회" << endl;
		cout << "4. 비밀번호 변경" << endl;
		cout << "5. 로그아웃" << endl;
		cout << "0. 종료" << endl;
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
	/* 내부 호출 함수: applyLecture, printStudent, printLectures, disapplyLecture, changePassword*/
	// 학생 메뉴 총괄 로그아웃(-1) 종료(1) 반환
	// 
}
