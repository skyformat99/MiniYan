#ifndef JNIUtil_h
#define JNIUtil_h

namespace yanbo
{
class JNIUtil
{
public:
	static void callStaticMethod(const char* className,
			const char* method,
			const char* signature,
			...
	);
};
}
#endif
