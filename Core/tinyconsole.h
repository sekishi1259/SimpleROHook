#pragma once

#define kDD_LOGGING(a) 
#define kDD_LOGGING2(a) DebugLogA a

#define kDD_LOGGING_TEST(a)

void DebugLogA(const char* format, ...);

void CreateTinyConsole(void);
void ReleaseTinyConsole(void);

