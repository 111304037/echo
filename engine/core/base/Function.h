#pragma once

#include "EchoDef.h"

// �Զ���sprintf,�Ƴ�ƽ̨��ؾ���
int lord_sprintf(char* buff, const int bufflen, const char* const format, ...);

// �Զ���sscanf,�Ƴ�ƽ̨��ؾ���
int lord_sscanf(const char* buff, const char* format, ...);