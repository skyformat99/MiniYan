// 只是一个简单的类测试
class Test
{
    prop mName;
    prop mPwd;
    prop mLen;

    function ctor() {
        mName = 11000;
        mPwd  = 0;
	    mLen  = 150;
    }

    function increment(len) {
	    var array = jsNew(Array);
        return len+1;
    }

    function getName(x, y, z) {
        mLen = increment(mLen);
	    log(mLen);
        return x+y+z;
    }

    function getPwd() {
        mName=mName+3;
        return mName;
    }
}

function jsNew(cls) {
    var obj = new(cls);
    obj.ctor();
    return obj;
}

function mul(left, right)
{
    return left * right;
}
function add(x, y, fn)
{
    return x+y+fn(x,y);
}
function main()
{
    log("main function begin");
    var yanbo=2, wo=10, hello, obj;
    wo=10;
    
	var testNum = 50;
	do {
        testNum=testNum-1;
        if (testNum==30) {
            break;
        }
    } while(testNum>0);
    
	hello=add(yanbo,wo,mul);
    wo=hello+3;
    obj=jsNew(Test);
    wo=obj.mName;
    wo=obj.getName(obj.mName,2,1000);
    log(wo);
    wo=obj.getPwd();
    wo=obj.mName+110;
    wo=wo + 1;
    log(wo);

    log("Test Array");

	var arr=jsNew(Array);
    arr.put("test main with my token");
    arr.put(10);
    arr.put(555);
	var test = "this js is perfect";
    arr.put(test);
    var res = arr.get(0);
	log(res);
    res = arr.get(1);
    log(res);
    res = arr.get(2);
    log(res);
	res = arr.get(3);
    log(res);

	log("Test Size");
	//var len = arr.size();
	log(arr.size());
	arr.clear();
	//len = arr.size();
	log(arr.size());

	var str = content("test.json");
	var testJson = json(str);
	log(testJson.retmsg);
	obj.mName=13333;
	log(obj.mName);
	getArea(testJson.data);
	log("main function end");
}

function getArea(data) {
    log("getArea");
	var app_version = data.app_version;
        var area = data.startup_info.area_list.get(0);
	//var area_list = startup_info.area_list;
	//var area = area_list.get(0);
	log(app_version.last);
	log(area.name);
}