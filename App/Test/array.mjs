// ���ģJS���
// �����������Ķ���
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