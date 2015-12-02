#ifndef SHARED_MD5_H
#define	SHARED_MD5_H

#include "types.h"

#ifndef PROTOTYPES
#define PROTOTYPES 0
#endif

/* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;

/* UINT2 defines a two byte word */
typedef unsigned short int UINT2;

/* UINT4 defines a four byte word */
typedef unsigned int UINT4;

#if PROTOTYPES
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif


class MD5
{
public:
	MD5(void);
	~MD5(void);
public:
	/* MD5 context. */
	typedef struct {
		UINT4 state[4];          /* state (ABCD) */
		UINT4 count[2];          /* number of bits, modulo 2^64 (lsb first) */
		unsigned char buffer[64];/* input buffer */
	} MD5_CTX;

public:
	void MD5Init(MD5_CTX * context);
	void MD5Update(MD5_CTX * context, unsigned char * input, unsigned int inputLen);
	void MD5Final(unsigned char digest[16], MD5_CTX *  context);

	/* extended function */
	int  MDFile(char *filename , unsigned char digest[16]);
	void MDPrint(unsigned char digest[16]);
	void MDString(char *str,unsigned char digest[16]);
	void MDData(char *data, int len,unsigned char digest[16]);

	void MD5Transform (UINT4 state[4], unsigned char block[64]);
	void Encode (unsigned char * output, UINT4 * input, unsigned int len);
	void Decode (UINT4 * output, unsigned char * input, unsigned int len);
	void MD5_memcpy (POINTER output, POINTER input, unsigned int len);
	void MD5_memset (POINTER output, int value, unsigned int len);

};

#endif	//SHARED_MD5_H
