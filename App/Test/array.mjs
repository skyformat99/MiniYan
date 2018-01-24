// 大规模JS编程
// 这个是数组类的定义
class Array {
    function ctor() {
    }

    function put(elem) {
        addInArray(this, elem);
    }

    function get(idx) {
        return getFromArray(this, idx, 5);
    }

	function size() {
	    return getArraySize(this, 5);
	}

	function clear() {
	    clearArray(this, 5);
	}
}