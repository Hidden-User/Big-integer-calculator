#include "pch.h"
#include "stdlib.h"
#include "stdio.h"
#include "memory.h"
//#include <iostream>

typedef char BYTE;

#ifdef WIN32

void* __malloc(size_t length) {
	void *vb = malloc(length + 4);
	return (void*)((unsigned)vb + 4);
}

void __free(void* vb) {
	free((void*)((unsigned)vb - 4));
}

#else

void* __malloc(size_t length) {
	return (void*)(unsigned long long(malloc(length + 4)) + 4);
}

void __free(void* vb) {
	free((void*)(unsigned long long(vb) - 4));
}

#endif // WIN32

BYTE* __atoiHEX(char* str) {
	size_t b = 8;
	int t = 0;
	int i = 0;
	int sign = 1;
	int lm = -1;
	BYTE* buff;
	if (str == NULL) {
		printf("");
		return NULL;
	}
	BYTE* res = (BYTE*)__malloc(b);
	if (str[0] == '-') {
		sign = -1;
		lm = 0;
	}
	for (; str[t] != 0; t++) {
	}
	t--;
	for (; t > lm; t--, i++) {
		if (i >= b) {
			buff = (BYTE*)__malloc(b * 2);
			for (unsigned g = 0; g < b; g++) {
				buff[g] = res[g];
			}
			b *= 2;
			__free(res);
			res = buff;
			buff = NULL;
		}
		res[i] = str[t] - 48;
		if (res[i] < 0) {
			printf("__atoiHEX::Incorrect symbol:%c", str[t]);
			free(res);
			res = NULL;
			break;
		}
		if (res[i] > 9) {
			res[i] -= 7;
			if (res[i] < 0) {
				printf("__atoiHEX::Incorrect symbol:%c", str[t]);
				free(res);
				res = NULL;
				break;
			}
		}
		if (res[i] > 15) {
			res[i] -= 32;
			if (res[i] < 0) {
				printf("__atoiHEX::Incorrect symbol:%c", str[t]);
				free(res);
				res = NULL;
				break;
			}
		}
		if (res[i] > 15) {
			printf("__atoiHEX::Incorrect symbol:%c", str[t]);
			free(res);
			res = NULL;
			break;
		}
	}
	if (i == b) {
		buff = (BYTE*)__malloc(b * 2);
		for (unsigned g = 0; g < b; g++) {
			buff[g] = res[g];
		}
		b *= 2;
		__free(res);
		res = buff;
		buff = NULL;
	}
	for (; i < b; i++) {
		res[i] = 0b01111111;
	}
	buff = res - 4;
	((int*)buff)[0] = i * sign;
	return res;
}

char* __itoaHEX(BYTE* big) {
	int* sBig = (int*)(big - 4);
	int sign;
	if (*sBig < 0) {
		sign = -1;
		*sBig *= -1;
	}
	else {
		sign = 1;
	}
	int t = *sBig - 1;
	int i = 0;
	char* res = (char*)malloc(sign > 0 ? (*sBig) + 1 : (*sBig) + 2);
	if (sign == -1) {
		res[0] = '-';
		i++;
	}
	for (; big[t] == 127; t--) {
	}
	for (; t > -1; t--, i++) {
		res[i] = big[t] < 10 ? big[t] + 48 : big[t] + 55;
	}
	if (sign == -1) {
		*sBig *= -1;
	}
	res[i] = '\0';
	return res;
}

BYTE* __newCopy(BYTE* big) {
	int *sBig = (int*)(big - 4);
	BYTE *res = (BYTE*)__malloc(*sBig);
	if ((int)res < 0) {
		return NULL;
	}
	memcpy_s(res - 4, *sBig + 4, big - 4, *sBig + 4);
	return res;
}

/*
big1 > big2 -> 1
big1 < big2 -> -1
big1 = big2 -> 0
*/
int __cmp(BYTE* big1, BYTE* big2) {
	int *sB1 = (int*)(big1 - 4);
	int *sB2 = (int*)(big2 - 4);
	int sign = (*sB1) * (*sB2);
	if (sign < 0) {
		if (*sB1 < 0) {
			return -1;
		}
		else {
			return 1;
		}
	}
	int t;
	int res = 0;

	if (*sB1 < 0) {
		sign = 1;
		*sB1 *= -1;
		*sB2 *= -1;
	}
	else {
		sign = 0;
	}

	if (*sB1 == *sB2) {
		for (t = *sB1 - 1; t > -1; t--) {
			if (big1[t] != 127 || big2[t] != 127) {
				if (big1[t] != 127 && big2[t] != 127) {
					if (big1[t] != big2[t]) {
						if (big1[t] > big2[t]) {
							res = 1;
						}
						else {
							res = -1;
						}
					}
				}
				else {
					if (big1[t] != big2[t]) {
						if (big1[t] != 127) {
							res = 1;
						}
						else {
							res = -1;
						}
					}
				}
			}
			if (res != 0) {
				break;
			}
		}
	}
	else {
		if (*sB1 > *sB2) {
			for (t = *sB1 - 1; t > *sB2 - 1; t--) {
				if (big1[t] != 127) {
					res = 1;
					break;
				}
			}
			if (res == 0) {
				for (; t > -1; t--) {
					if (big1[t] != 127 || big2[t] != 127) {
						if (big1[t] != 127 && big2[t] != 127) {
							if (big1[t] != big2[t]) {
								if (big1[t] > big2[t]) {
									res = 1;
								}
								else {
									res = -1;
								}
							}
						}
						else {
							if (big1[t] != big2[t]) {
								if (big1[t] != 127) {
									res = 1;
								}
								else {
									res = -1;
								}
							}
						}
					}
					if (res != 0) {
						break;
					}
				}
			}
		}
		else {
			for (t = *sB2 - 1; t > *sB1 - 1; t--) {
				if (big2[t] != 127) {
					res = -1;
					break;
				}
			}
			if (res == 0) {
				for (; t > -1; t--) {
					if (big1[t] != 127 || big2[t] != 127) {
						if (big1[t] != 127 && big2[t] != 127) {
							if (big1[t] != big2[t]) {
								if (big1[t] > big2[t]) {
									res = 1;
								}
								else {
									res = -1;
								}
							}
						}
						else {
							if (big1[t] != big2[t]) {
								if (big1[t] != 127) {
									res = 1;
								}
								else {
									res = -1;
								}
							}
						}
					}
					if (res != 0) {
						break;
					}
				}
			}
		}
	}

	if (sign) {
		*sB1 *= -1;
		*sB2 *= -1;
		res *= -1;
	}
	return res;
}

void add(BYTE*& dst, BYTE* src);
void sub(BYTE*& dst, BYTE* src);

// dst + src -> dst
void add(BYTE*& dst, BYTE* src) {
	int t = 0;
	BYTE bb = 0;
	BYTE* buff = NULL;
	int* sDst = (int*)(dst - 4);
	int* sSrc = (int*)(src - 4);
	BYTE* _dstBuff = NULL;
	BYTE* _srcBuff = NULL;
	int sign = (*sDst) * (*sSrc);
	if (sign < 0) {
		if (*sDst < 0) {
			*sDst *= -1;
			_dstBuff = __newCopy(src);
			_srcBuff = __newCopy(dst);
			sub(_dstBuff, _srcBuff);
			__free(_srcBuff);
			__free(dst);
			dst = _dstBuff;
		}
		else {
			*sSrc *= -1;
			_dstBuff = __newCopy(dst);
			sub(_dstBuff, src);
			__free(dst);
			dst = _dstBuff;
		}
		return;
	}
	else {
		sign /= sign;
	}
	if (*sDst < 0) {
		sign *= -1;
		*sDst *= -1;
		*sSrc *= -1;
	}
	for (; (dst[t] < 127) && (src[t] < 127); t++) {
		bb += dst[t] + src[t];
		dst[t] = bb % 16;
		bb /= 16;
	}
	if (src[t] == 127) {
		for (; bb != 0; t++) {
			if (t == *sDst) {
				buff = (BYTE*)__malloc(*sDst * 2);
				for (int g = 0; g < *sDst; g++) {
					buff[g] = dst[g];
				}
				for (int g = *sDst; g < (*sDst * 2); g++) {
					buff[g] = 127;
				}
				sDst = (int*)(buff - 4);
				*sDst = (int)(*(dst - 4)) * 2;
				__free(dst);
				dst = buff;
				buff = NULL;
			}
			dst[t] = bb % 16;
			bb /= 16;
		}
		if (t != *sDst) {
			for (; dst[t] != 127; t++) {
			}
		}
	}
	else {
		for (; src[t] < 127; t++) {
			if (t == *sDst) {
				buff = (BYTE*)__malloc(*sDst * 2);
				for (int g = 0; g < *sDst; g++) {
					buff[g] = dst[g];
				}
				for (int g = *sDst; g < (*sDst * 2); g++) {
					buff[g] = 127;
				}
				sDst = (int*)(buff - 4);
				*sDst = (int)(*(dst - 4)) * 2;
				__free(dst);
				dst = buff;
				buff = NULL;
			}
			bb += src[t];
			dst[t] = bb % 16;
			bb /= 16;
		}
	}
	if (t == *sDst) {
		buff = (BYTE*)__malloc(*sDst * 2);
		for (int g = 0; g < *sDst; g++) {
			buff[g] = dst[g];
		}
		for (int g = *sDst; g < (*sDst * 2); g++) {
			buff[g] = 127;
		}
		sDst = (int*)(buff - 4);
		*sDst = (int)(*(dst - 4)) * 2;
		__free(dst);
		dst = buff;
		buff = NULL;
	}
	for (; t < *sDst; t++) {
		dst[t] = 127;
	}
	if (sign < 0) {
		*sDst *= -1;
		*sSrc *= -1;
	}
}

// dst - src -> dst
void sub(BYTE*& dst, BYTE* src) {

	BYTE* buff = NULL;
	int* sDst = (int*)(dst - 4);
	int* sSrc = (int*)(src - 4);
	BYTE* _dstBuff = NULL;
	BYTE* _srcBuff = NULL;
	int resSign = 0;
	int t = 0;
	BYTE bb = 0;

	if (((*sDst) * (*sSrc)) < 0) {
		if (*sDst < 0) {
			*sDst *= -1;
			_dstBuff = __newCopy(dst);
			add(_dstBuff, src);
			__free(dst);
			dst = _dstBuff;
			sDst = (int*)(dst - 4);
			*sDst *= -1;
		}
		else {
			*sSrc *= -1;
			_dstBuff = __newCopy(dst);
			add(_dstBuff, src);
			__free(dst);
			dst = _dstBuff;
			sDst = (int*)(dst - 4);
			*sSrc *= -1;
		}
		return;
	}

	if (*sDst < 0) {
		*sDst *= -1;
		*sSrc *= -1;
		bb = 1;
	}
	_dstBuff = __newCopy(dst);
	_srcBuff = __newCopy(src);
	if (bb) {
		*sDst *= -1;
		*sSrc *= -1;
		bb = 0;
	}

	switch (__cmp(_dstBuff, _srcBuff))
	{
	default:
		printf("Big int error sub -> __cmp");
		bb = 1;
		break;
	case 0:
		__free(_dstBuff);
		__free(_srcBuff);
		if (*sDst < 0) {
			*sDst *= -1;
		}
		for (; t < *sDst; t++) {
			dst[t] = 127;
		}
		dst[0] = 0;
		return;
	case 1:
		//dst > src
		if (*sDst < 0) {
			//buff = _srcBuff;
			//_srcBuff = _dstBuff;
			//_dstBuff = buff;
			//buff = NULL;
			*sDst *= -1;
			resSign = -1;
		}
		else {
			resSign = 1;
		}
		break;
	case -1:
		//dst < src
		buff = _srcBuff;
		_srcBuff = _dstBuff;
		_dstBuff = buff;
		buff = NULL;
		if (*sDst < 0) {
			*sDst *= -1;
			resSign = 1;
		}
		else {
			resSign = -1;
		}
		break;
	}

	if (bb) {
		printf("sub was stoped");
		__free(_dstBuff);
		__free(_srcBuff);
		return;
	}

	for (; _srcBuff[t] < 127; t++) {
		if (t >= *sDst) {
			buff = (BYTE*)__malloc((*sDst) * 2);
			memcpy(buff - 4, dst - 4, *sDst + 4);
			for (int g = *sDst; g < *sDst * 2; g++) {
				buff[g] = 127;
			}
			__free(dst);
			dst = buff;
			buff = NULL;
			sDst = (int*)(dst - 4);
			*sDst *= 2;
		}
		dst[t] = _dstBuff[t] - (_srcBuff[t] + bb);
		bb = 0;
		if (dst[t] < 0) {
			dst[t] += 16;
			bb = 1;
		}
	}
	if (_dstBuff[t] != 127) {
		if (bb) {
			dst[t] = _dstBuff[t] - bb;
			t++;
		}
		for (; _dstBuff[t] < 127; t++) {
			if (t >= *sDst) {
				buff = (BYTE*)__malloc((*sDst) * 2);
				memcpy(buff - 4, dst - 4, *sDst + 4);
				for (int g = *sDst; g < *sDst * 2; g++) {
					buff[g] = 127;
				}
				__free(dst);
				dst = buff;
				buff = NULL;
				sDst = (int*)(dst - 4);
				*sDst *= 2;
			}
			dst[t] = _dstBuff[t];
		}
		t--;
		for (; dst[t] == 0; t--) {
		}
		t++;
		for (; t < *sDst; t++) {
			dst[t] = 127;
		}
	}
	else {
		t--;
		for (; dst[t] == 0; t--) {
		}
		t++;
		for (; t < *sDst; t++) {
			dst[t] = 127;
		}
	}

	__free(_dstBuff);
	__free(_srcBuff);

	*sDst *= resSign;

}

// dst * src -> dst
void mul(BYTE*& dst, BYTE* src) {
	int t = 0;
	int i;
	BYTE* buff = NULL;
	BYTE* res = (BYTE*)__malloc(8);
	for (int t = 0; t < 8; t++) {
		res[t] = 0;
	}
	int* sDst = (int*)(dst - 4);
	int* sSrc = (int*)(src - 4);
	int* sRes = (int*)(res - 4);
	*sRes = 8;
	BYTE bb = 0;
	int ib = 0;
	int sign = (*sDst) * (*sSrc);
	if (sign < 0) {
		sign /= -sign;
	}
	else {
		sign /= sign;
	}
	*sDst = *sDst > 0 ? (*sDst) : -(*sDst);
	*sSrc = *sSrc > 0 ? (*sSrc) : -(*sSrc);
	for (; src[t] < 127; t++) {
		i = 0;
		for (; dst[i] < 127; i++) {
			if ((t + i) >= *sRes) {
				buff = (BYTE*)__malloc(*sRes * 2);
				for (int g = 0; g < *sRes; g++) {
					buff[g] = res[g];
				}
				for (int g = *sRes; g < (*sRes * 2); g++) {
					buff[g] = 0;
				}
				sRes = (int*)(buff - 4);
				*sRes = (int)(*(res - 4)) * 2;
				__free(res);
				res = buff;
				buff = NULL;
			}
			ib += (dst[i] * src[t]) + res[t + i];
			res[t + i] = ib % 16;
			ib /= 16;
		}
		for (; ib != 0; i++) {
			if ((t + i) >= *sRes) {
				buff = (BYTE*)__malloc(*sRes * 2);
				for (int g = 0; g < *sRes; g++) {
					buff[g] = res[g];
				}
				for (int g = *sRes; g < (*sRes * 2); g++) {
					buff[g] = 0;
				}
				sRes = (int*)(buff - 4);
				*sRes = (int)(*(res - 4)) * 2;
				__free(res);
				res = buff;
				buff = NULL;
			}
			res[t + i] = ib % 16;
			ib /= 16;
		}
	}
	t += i - 1;
	for (; t < *sRes; t++) {
		res[t] = 127;
	}
	*sRes *= sign;
	__free(dst);
	dst = res;
}

int main(int argc, char** argv)
{
	if (argc == 1) {
		printf("num1 (+,-,*) num2");
		return 0;
	}
	if (argc < 4) {
		printf("Not enought arguments for calculating");
		return 0;
	}
	if (argc > 4) {
		printf("The number of arguments exceeds the necessary value(3)\n");
		printf("Trying to work with first three arguments\n");
	}
	BYTE* a = __atoiHEX(argv[1]);
	BYTE* b = __atoiHEX(argv[3]);
	if (a == NULL) {
		printf("1st argument is not a number");
		return 0;
	}
	if (b == NULL) {
		printf("3l argument is not a number");
		return 0;
	}
	switch (argv[2][0])
	{
	default:
		printf("unknow action, return 1st argument\n");
		break;
	case '+':
		add(a, b);
		break;
	case '-':
		sub(a, b);
		break;
	case '*':
		mul(a, b);
		break;
	}
	char* out = __itoaHEX(a);
	printf("%s\n", out);
	__free(a);
	__free(b);
	free(out);
	return 0;
}
