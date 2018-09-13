2018-06-08  至 2018-06-09  （一共写了大概12h）   
感觉是我写过最复杂的玩意了，另外一开始要求也定的不低，后来写到快崩溃，结果还是硬给磨下来了。  
CMD_Parse是同学写的。  

题目描述见 Yamlite.pdf  
记录一下想法，免得到时候看不懂了：  
整体思路就是每行每行扫，先归个类，然后再整段分析。  

1. 预处理： `_STD tuple<std::size_t, const char*, std::size_t>
		preprocess(std::size_t line_num, const char* c)`  
这个函数的作用是：忽略注释，并且检查空格段的数量（除了开头的最多一个），然后将结尾的空格也都忽略。  

2. 扫描一行： `void seperate_one_line(std::size_t line_num, const char* c, std::size_t size)`  
事实上，每一行只有4种类型：  

        /*
         * kv       : 8, "key: value" key: string(value ""), number, scientific_notation, bool
         * key      : 4, "key:"       kv (but value is array or kv)
         * value    : 2, "- value"    trivial value type of the array
         * new_line : 1, "-"          to start a new line, indicating the value is "kv" or "array"
        **/
        enum class line_t { kv = 8, key = 4, value = 2, new_line = 1 };
  
先统计有多少个 indent (1个 indent 为2个 space)，然后再看下一个是啥
如果是 ‘-’，就说明是 value 或 new_line， 否则尝试解释为 key 或 kv。   
最主要的工作全部由两个函数 `key_scan` 和 `value_scan` 完成。   
（注：科学计数法那里写的不是很完善(..•˘_˘•..)，没怎么做异常处理）  

3.&nbsp;解析： `void parsing(option opt, const char* search_path)`  
先做 `-parse`，这一块的大致思路是：  
遍历所有行，用一个栈来存储 indent，如果小于栈顶的 indent，那么尝试 reduce。  
这里面有2个结构：  

	using _key = std::string;  
	using _value = std::string;  
	_STD unordered_map<_key, _value> find_table;
find_table 用来存储所有的值 与其对应的目录，存储过程是在扫描中进行的。也就是说，`-parse` 之后，如果 option 为 `-find`，只需要在 find_table 中查找 search_path 即可。  
	
	typedef struct path {
		YAMLlite::line l;
		std::string cur_path;
		std::string parent_path;
		int array_level;// -1 if no - array, store array_level for reduce
		path(YAMLlite::line _l, const std::string& _cur_path, const std::string& _parent_path, int ary_level = -1)
		:l(_l), cur_path(_cur_path), parent_path(_parent_path), array_level(ary_level)
		{}
	};
	_STD stack<path> stk;
stk 用来存储每一行的信息，包括本行信息，以及当前目录和父目录，还有数组的层次。stk中的元素都位于同一段中，段结尾处全部 reduce。   
其实一开始只有 key 和 new_line 要存储当前目录，但是后来改了一下，存储所有行，于是对于 kv 或 value，就存储父目录(当然这两个同时也要根据上一项的（当前/父）目录来存储进 find_table )。   
我做了一个表来表示每一行可接受的状态：  
>
		/*
		 *  0和1指的是这一行与上一行缩进的差， 若 < 0 记为 -1, then reduce
		 *  左列是上一行的类型，值是这一行可以接受的类型。
		 *  ______________________________________________
		 *  |_________________|    0   |    1   |   -1   |
		 *  |  kv       : 8   |  0000  |  0000  |  1111  |   (kv, key) must have 0 indent!!!
		 *  |  key      : 4   |  0000  |  1111  |  0000  |
		 *  |  value    : 2   |  0011  |  0000  |  1111  |   (kv, key) must have 0 indent!!!
		 *  |  new_line : 1   |  0000  |  1111  |  0000  |
		 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		**/

举例：如果上一行是 value，并且这一行的 indent 多了 1，那么结果就是 0000 报错；如果这一行的 indent 与上一行同级，则结果是 0011，即这一行只能是 value 或 new_line 类型。  
具体写起来是这样：首先看 stk 是否为空，即开始新的一段。  
当 stk 非空时，比较这一行与上一行的缩进，分为三种情况：  
>
1. 相同   
2. 多1层缩进   
3. 少于上一行，那么尝试 reduce   

其实1和2大同小异，就是先查表，然后再根据可能的情况进行分析。中间有个地方比较累，就是 stk 的记录，因为一开始我想的是 只记录 key 和 new_line，因为这两个才算是新的目录，但后来写的时候忘了，stk记录的是每一行，所以又强行记录了 kv 和 value。为此我还专门为 find_table 和 stk 封装了4个函数，专门用来记录。一开始 stk 存的信息还少，后来加了记录函数，又重构了两次后就清晰多了。只要搞清楚什么情况下选择 "当前目录" 还是 "父目录" 就好。   
至于3 reduce，还是先查表，然后 stk 不断弹出，直到与当前 indent 同级别。（当然，注意到：如果最后 stk 只剩1个元素，即判断为开启新的一段，将最后一个元素手动弹出）看表的话虽然4种情况都是会出现，但其实还是可以更进一步分层，我在表后面的注释已经写了。如果 stk 为空，那么新的一行只能是 kv 或 key，其他就是非空的情况了。 其实这一块我偷了个懒，**讲道理我觉得一个分析器应该把这个结构记下来，也就是当 reduce 一整段的时候应该把这一段的全部信息存储到某个结构中，并且这种数据结构可以完整地刻画出这一段的信息。**但因为作业中要求的操作只有 `-find`，而所有 value 和 其对应的目录 已经被记录在 find_table 中，所以我就没写。。。 (￣ェ￣;)    

然后就只剩下 `-json` 的部分了，这个部分看起来就可以和之前解耦了，因为已经通过了 `-parse` ，所以格式已经保证正确，写起来也好受一些。  
主要思路：还是遍历所有行，同时用一个栈来存储 右括号 `' ) '` 和 `' } '` 的 indent，然后在合适的时机弹出栈。（大概要注意的就是按照给的样例格式来，注意一下逗号和换行就好；逗号就是如果下面还有同 indent 的，那么有逗号）

  
以上。   

总之我觉得自己写得很6。   
｡:.ﾟヽ(｡◕‿◕｡)ﾉﾟ.:｡+ﾟ  
最后，今早听说 Ranges TS 要进C++20，感觉完全跟不上时代。。。( ˘•ω•˘ ) 