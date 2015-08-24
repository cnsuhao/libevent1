#ifndef PLATFORMTESTAPI_WIN32_H
#define PLATFORMTESTAPI_WIN32_H

#define TESTCASE_DEFINE_IMPLE_(TestCaseName)    void DummyTestCaseFunction##TestCaseName (){} \
												extern "C" __declspec(dllexport)  void TestCaseFunction##TestCaseName ()

#define TESTCASE_DECLARE_IMPLE_(TestCaseName)  void DummyTestCaseFunction##TestCaseName (); \
	                                           DummyTestCaseFunction##TestCaseName ()

#endif