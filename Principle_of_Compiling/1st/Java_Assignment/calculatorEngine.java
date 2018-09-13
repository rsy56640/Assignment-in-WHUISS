import java.io.IOException;
import java.io.Reader;
import java.io.StringWriter;
import java.io.Writer;
import java.util.*;
import  javax.script.*;

public class calculatorEngine extends AbstractScriptEngine
{
	@Override
	public Object eval(String script, ScriptContext context) throws ScriptException{
		// TODO Auto-generated method stub
		String identifier=new String();
		Boolean isValid=false;
		int i=0;
		for(;i<script.length();++i)
		{
			if(script.charAt(i)==';')
			{
				isValid=true;
				break;
			}
		}
			try {
				if(!isValid)throw new TokenException("loss of semicolon",0,i);
				String str=script.substring(0, i+1);
				identifier=execution_one_sentence(seperate_one_sentence(str));
			} catch (TokenException | IOException e) {
				throw new ScriptException(e);
			}
		if(identifier!="")
			context.setAttribute(identifier, symbol_table.get(identifier), ScriptContext.GLOBAL_SCOPE);
		return null;
	}
	
	@Override  
	public void put(String s, Object o){  
		symbol_table.put(s,Double.valueOf(o.toString()));  
	} 
	
	@Override
	public Object eval(Reader reader, ScriptContext context) throws ScriptException {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Bindings createBindings() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public ScriptEngineFactory getFactory() {
		// TODO Auto-generated method stub
		return new calculatorEngineFactory();
	}
	
	


	public static void main(String args[]) throws IOException
	{
		final Integer num = 4;
		String c[]=new String[num];
		c[0] = "a=(10.44*356+1.28)/2+1024*1.6;";
				//"a=(10.44*  356  +  1.28) /2+  1024 *1.6;";
		c[1] = "b=a*2-a/2;";
				//" b=a*2-a/2;";
		c[2] = "print(b);";
		c[3] = "print(a);";

		Integer index[]=new Integer[num];
		for(int i=0;i<num;++i)index[i]=0;
		for (Integer i = 0; i < num; ++i)
			for (; c[i].charAt(index[i]) != ';';)
			{
				index[i]=index[i]+1;
			}
		Vector<Vector<Token>> v = new Vector<Vector<Token>>();
		try
		{
			for (Integer i = 0; i < num; ++i)
			{
				String str=c[i].substring(0, index[i]+1);
				v.add(seperate_one_sentence(str));
				new calculatorEngine().execution_one_sentence(v.get(i));
			}

		}
		catch (TokenException e)
		{
			e.printException();
		}
	}//end main();
	
	public static Integer sentence_num=0;
	public static HashMap<String, Double> symbol_table = new HashMap<String, Double>();
	//public Bindings symbol_table=createBindings();
	//one sentence including the last semicolon
	static public Vector<Token> seperate_one_sentence(String sentence) throws TokenException
	{
		sentence_num++;
		final Integer[] SET_VALUES_FOR_DIGIT = new Integer[] 
				{
					1, 2, 3, 4, 5, 6, 7, 8, 9, 0
				};
		final HashSet<Integer> digit = new HashSet<Integer>(Arrays.asList(SET_VALUES_FOR_DIGIT));
		final String[] SET_VALUES_FOR_OP = new String[] 
				{
					"+", "-", "*", "/", "=", "(", ")"
				};
		final HashSet<String> op = new HashSet<String>(Arrays.asList(SET_VALUES_FOR_OP));
		final String[] SET_VALUES_FOR_WORD = new String[] 
				{
					"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", 
					"n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
					"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", 
					"N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
					"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "$", "_"
				};
		final HashSet<String> word = new HashSet<String>(Arrays.asList(SET_VALUES_FOR_WORD));
		Vector<Token> token_list = new Vector<Token>();
		final Integer length = sentence.length()-1;
		int cur_pos = 0;
		while (cur_pos < length)
		{
			if (sentence.charAt(cur_pos) == ' ')
				{
					cur_pos++;
					continue;
				}
			Boolean is_const=false;
			final Integer cur_pos_for_const=cur_pos;
			while(digit.contains(Character.getNumericValue(sentence.charAt(cur_pos)))
					||sentence.charAt(cur_pos)=='.')
			{
				is_const=true;
				cur_pos++;
			}
			if(is_const)
			{
				String str =sentence.substring(cur_pos_for_const,cur_pos);
				Double num=0.0;
					try
					{
						num=Double.parseDouble(str);
					}catch(NumberFormatException e)
					{
						throw new TokenException("Wrong with the num",sentence_num,cur_pos);
					}
				token_list.add(new Token(1,"",num));
				continue;
			}
			if (op.contains(String.valueOf(sentence.charAt(cur_pos))))
			{
				if (sentence.charAt(cur_pos) == '=')
				{
					token_list.add(new Token(4, "="));
					cur_pos++;
					continue;
				}
				if (sentence.charAt(cur_pos) == '(')
				{
					token_list.add(new Token(5,"("));
					cur_pos++;
					continue;
				}
				if (sentence.charAt(cur_pos) == ')')
				{
					token_list.add(new Token(6,")"));
					cur_pos++;
					continue;
				}
				token_list.add(new Token(2,sentence.substring(cur_pos,cur_pos+1)));
				cur_pos++;
				continue;
			}
			Boolean is_word = false;
			final Integer cur_pos_for_word=cur_pos;
			while (word.contains(String.valueOf(sentence.charAt(cur_pos))))
			{
				is_word = true;
				cur_pos++;
			}
			if (is_word)
			{
				String str = sentence.substring(cur_pos_for_word,cur_pos);
				if (str.equals("print"))
				{
					token_list.add(new Token(3,str));
					continue;
				}
				token_list.add(new Token(0,str));
				continue;
			}
			throw new TokenException("unexpected operator"+sentence.substring(cur_pos,cur_pos+1),
					sentence_num,cur_pos);
		}//end while
		return token_list;
	}//end function seperate_one_sentence();
	
	
	public String execution_one_sentence(Vector<Token> token_list) throws TokenException, IOException
	{
		final Integer size = token_list.size();
		if (0 == size)return "";
		if (token_list.get(0)._ty == 0)
		{
			if(size==1)
				throw new TokenException("Too little information",sentence_num,1);
			if(token_list.get(1)._ty != 4)
				throw new TokenException("Unexpected token",sentence_num,1);
			Vector<Token> expr = new Vector<Token>(token_list.size());  
			expr.setSize(token_list.size());
		    Collections.copy(expr,token_list);
			expr.remove(1);expr.remove(0);
			Double value=calculate(expr,2);
			symbol_table.put(token_list.get(0)._symbol, value);
			return token_list.get(0)._symbol;
		}
		if(token_list.get(0)._ty == 3)
		{
			if(size<4
				||token_list.get(1)._ty!=5
				||token_list.get(size-1)._ty!=6)
				throw new TokenException("unexpected token", sentence_num, size-1);
			Vector<Token> expr = new Vector<Token>(size);  
			expr.setSize(token_list.size());
		    Collections.copy(expr,token_list);
			expr.remove(size-1);expr.remove(1);expr.remove(0);
			Double value=calculate(expr,2);
			Writer wr=this.getContext().getWriter();
			//Writer wr=new calculatorEngineFactory().getScriptEngine().getContext().getWriter();
			wr.write(value+"\n");
			//System.out.println(value);
			return"";
		}
		throw new TokenException("unecpected token ",sentence_num,0);
	}
	
	static public Double calculate(Vector<Token> expr_list,Integer pos) throws TokenException
	{
		final Integer size = expr_list.size();
		Integer cur_pos = 0;
		Boolean next_identifer = true;
		Integer left_parenthesis = 0, right_parenthesis = 0;
		while (cur_pos < size)
		{
			if (expr_list.get(cur_pos)._ty == 5)
			{
				cur_pos++;
				left_parenthesis++;
				next_identifer = true;
				continue;
			}
			if (expr_list.get(cur_pos)._ty == 6)
			{
				if (next_identifer)
					throw new TokenException("There is something wrong near",
							sentence_num, cur_pos + pos);
				cur_pos++;
				right_parenthesis++;
				next_identifer = false;
				continue;
			}
			if (expr_list.get(cur_pos)._ty < 2)
			{
				if (!next_identifer)
					throw new TokenException("There is something wrong near",
							sentence_num, cur_pos + pos);
				cur_pos++;
				next_identifer = false;
				continue;
			}
			if (expr_list.get(cur_pos)._ty == 2)
			{
				if (next_identifer)
					throw new TokenException("There is something wrong near",
							sentence_num, cur_pos + pos);
				cur_pos++;
				next_identifer = true;
				continue;
			}
		}
		if (next_identifer)
			throw new TokenException("unexpected end of the expression",
					sentence_num, cur_pos + pos);
		if (left_parenthesis != right_parenthesis)
			throw new TokenException("Parentheses does not match",
					sentence_num, cur_pos + pos);
		class Operator
		{
			public String _str;
			public Integer _priority;
			public Operator(String str, Integer priority)
			{
				_str=str;
				_priority=priority;
			}
		}
		Stack<Double> number_stack = new Stack<Double>();
		Stack<Operator> operator_stack = new Stack<Operator>();
		cur_pos=0;
		while (cur_pos < size)
		{
			Token token = expr_list.get(cur_pos);
			if (token._ty == 0)
			{
				Double num = symbol_table.get(token._symbol);
				if (num != null)
				{
					cur_pos++;
					number_stack.push(num);
				}
				else
					throw new TokenException("There is no such identifier: "+token._symbol, 
							sentence_num, cur_pos + pos);
				continue;
			}
			if (token._ty == 1)
			{
				cur_pos++;
				number_stack.push(token._num);
				continue;
			}
			if(token._ty == 2)
			{
				cur_pos++;
				final String cur_str = token._symbol;
				Integer cur_priority=2;
				if(cur_str.equals("+")||cur_str.equals("-"))cur_priority=1;
				if(operator_stack.empty())
				{
					if(cur_priority==1)
						operator_stack.push(new Operator(cur_str,1));
					else operator_stack.push(new Operator(cur_str,2));
					continue;
				}
				if (operator_stack.peek()._priority == 3)
				{
					operator_stack.push(new Operator(cur_str, cur_priority));
					continue;
				}
				final Integer pre_priority=operator_stack.peek()._priority;
				final String pre_op=operator_stack.peek()._str;
				if (cur_priority > pre_priority)
					operator_stack.add(new Operator(cur_str, cur_priority));
				else
				{
					Double num2 = number_stack.pop();
					Double num1 = number_stack.pop();
					operator_stack.pop();
					Double result = 0.0;
					switch (pre_op)
					{
					case "+":
						result = num1 + num2;
						break;
					case "-":
						result = num1 - num2;
						break;
					case "*":
						result = num1 * num2;
						break;
					case "/":
						if (Math.abs(num2) < 1e-9)
							throw new TokenException("Divided by zero", sentence_num, pos + cur_pos);
						result = num1 / num2;
						break;
					}
					operator_stack.push(new Operator(cur_str, cur_priority));
					number_stack.push(result);
				}
				continue;
			}
			if(token._ty == 5)
			{
				cur_pos++;
				operator_stack.push(new Operator("(",3));
				continue;
			}
			if(token._ty == 6)
			{
				while (!operator_stack.empty() && operator_stack.peek()._str != "(")
				{
					final String op = operator_stack.pop()._str;
					final Double num2 = number_stack.pop();
					final Double num1 = number_stack.pop();
					Double result = 0.0;
					switch (op)
					{
					case "+":
						result = num1 + num2;
						break;
					case "-":
						result = num1 - num2;
						break;
					case "*":
						result = num1 * num2;
						break;
					case "/":
						if (Math.abs(num2) < 1e-9)
							throw new TokenException("Divided by zero", sentence_num, pos + cur_pos);
						result = num1 / num2;
						break;
					}
					number_stack.push(result);
				}
				if (operator_stack.empty())
					throw new TokenException("unexpected right parenthesis ')'", 
							sentence_num, cur_pos + pos);
				//if top = '( '
				else
				{
					operator_stack.pop();
					cur_pos++;
					continue;
				}
			}
		}
		while (!operator_stack.empty())
		{
			Operator op = operator_stack.pop();
			Double num2 = number_stack.pop();
			Double num1 = number_stack.pop();
			Double result = 0.0;
			switch (op._str)
			{
			case "+":
				result = num1 + num2;
				break;
			case "-":
				result = num1 - num2;
				break;
			case "*":
				result = num1 * num2;
				break;
			case "/":
				if (Math.abs(num2) < 1e-9)
					throw new TokenException("Divided by zero", sentence_num, pos + cur_pos);
				result = num1 / num2;
				break;
			}
			number_stack.push(result);
		}
		return number_stack.pop();
	}
}//end class 

class Token
{
	public static final int identifier = 0; 
	public static final int constant = 1;
	public static final int opcode = 2;
	public static final int print = 3;
	public static final int equal = 4;
	public static final int lp = 5;
	public static final int rp = 6;
	public Token(int type, String symbol)
	{
		this(type,symbol,0.0);
	}
	public Token(int type, String symbol, Double num)
	{
		_ty=type;
		_symbol=symbol;
		_num=num;
	}
	public int _ty;
	public String _symbol;	//id, op, lp, rp, semicolon
	public Double _num;			//const
}

class TokenException extends Exception
{
	public TokenException(String msg, Integer sentence_num, Integer pos)
	{
		_msg=msg;
		_sentence_num=sentence_num;
		_pos=pos;
	}
	public void printException()
	{
		System.out.println(_msg+" in the sentence: "+_sentence_num+", at the position: "+_pos);
	}
	private String _msg;
	private Integer _sentence_num;
	private Integer _pos;
}