/**
	Failure exception.
**/
#pragma once
#ifndef FAIL_H
#define FAIL_H

#include <exception>

using std::exception;

class Failure: public exception {
	const char *what_s;
public:
	Failure(const char *what): what_s(what) {};
	~Failure() { delete what_s; };
	const char* what() { return what_s; };
};

#endif FAIL_H