#ifndef GLView_h
#define GLView_h

namespace yanbo
{

class GLView
{
public:
	enum TouchEventType
	{
	    ACTION_DOWN = 0,
	    ACTION_UP = 1,
	    ACTION_MOVE = 2,
	    ACTION_CANCEL = 3,
	    ACTION_OUTSIDE,
	    ACTION_POINTER_DOWN,
	    ACTION_POINTER_UP,
	};

	struct MotionEvent
	{
        int mType;
        int mX;
        int mY;
        int mZ;
	};

	GLView();
	virtual ~GLView();

public:
	virtual void handleTouchEvent(const MotionEvent& event);
};

}
#endif
