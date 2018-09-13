import javax.script.ScriptEngine;
import javax.script.ScriptEngineManager;
import javax.script.ScriptException;

import java.io.StringWriter;

import static java.util.Objects.nonNull;

/**
 * 2018年软件工程-编译技术及应用-第一次作业评测程序
 */
public class TestCalculatorScript {
    private static final ScriptEngineManager SCRIPT_ENGINE_MANAGER = new ScriptEngineManager();
    private static int score = 0;
    private static boolean jsr223Supported = false;
    private static final String SCRIPT_ENGINE_NAME = "calculator";

    public static void main(String[] args) {
        System.out.println("=====================================");
        testJSR223();
        testArithmetic();
        testVariable();
        testPrintValue();
        testErrors();
        System.out.println("=====================================");
        System.out.println("Scores: " + score);
    }

    //1.JSR223的支持（20分）
    private static void testJSR223() {
        String name = "JSR223 Support";
        ScriptEngine calculator = SCRIPT_ENGINE_MANAGER.getEngineByName(SCRIPT_ENGINE_NAME);
        if (nonNull(calculator)) {
        	jsr223Supported = true;
            score += 20;
            pass(name);
        } else {
            failed(name);
        }
    }

    //2.算术运算和赋值（40分）
    private static void testArithmetic() {
        String name = "Arithmetic";
        if (jsr223Supported) {
            try {
                ScriptEngine calculator = SCRIPT_ENGINE_MANAGER.getEngineByName(SCRIPT_ENGINE_NAME);
                calculator.eval("result=(10.44*356+1.28)/2+1024*1.6;");
                Object result = calculator.getContext().getAttribute("result");
                if (result instanceof Double
                        && ((Double) result).compareTo((10.44 * 356 + 1.28) / 2 + 1024 * 1.6) == 0) {
                    score += 40;
                    pass(name);
                } else {
                    failed(name);
                }
            } catch (ScriptException e) {
                failed(name);
            }
        } else {
            failed(name);
        }
    }

    //3.变量支持（20分）
    private static void testVariable() {
        String name = "Variable";
        if (jsr223Supported) {
            try {
                ScriptEngine calculator = SCRIPT_ENGINE_MANAGER.getEngineByName(SCRIPT_ENGINE_NAME);
                calculator.put("c", 2333);
                calculator.eval("a=(10.44*356+1.28)/2+1024*1.6;");
                calculator.eval("b=a*2-a/2;");
                calculator.eval("result=b+c;");
                Object result = calculator.getContext().getAttribute("result");
                double a = (10.44 * 356 + 1.28) / 2 + 1024 * 1.6;
                double b = a * 2 - a / 2;
                if (result instanceof Double && ((Double) result).compareTo(b + 2333) == 0) {
                    score += 20;
                    pass(name);
                }
            } catch (ScriptException e) {
                failed(name);
            }
        } else {
            failed(name);
        }
    }

    //4.打印（10分）
    private static void testPrintValue() {
        String name = "print";
        if (jsr223Supported) {
            try {
                ScriptEngine calculator = SCRIPT_ENGINE_MANAGER.getEngineByName(SCRIPT_ENGINE_NAME);
                StringWriter out = new StringWriter();
                calculator.getContext().setWriter(out);//重定向输出流
                calculator.eval("print(233);");
                calculator.eval("print(1+2+(3+4));");
                String result = out.toString();
                if (result.equals("233.0\n10.0\n")) {
                    score += 10;
                    pass(name);
                } else {
                    failed(name);
                }
            } catch (ScriptException e) {
                failed(name);
            }
        } else {
            failed(name);
        }
    }

    //5.错误处理（10分）
    private static void testErrors() {
        String name = "Errors";
        if (jsr223Supported) {
            ScriptEngine calculator = SCRIPT_ENGINE_MANAGER.getEngineByName(SCRIPT_ENGINE_NAME);
            try {
                calculator.eval("a=2333");//缺号分号
                failed(name);
            } catch (ScriptException e) {
                score += 3;
                try {
                    calculator.eval("a=hhh;");//未定义的变量
                    failed(name);
                } catch (ScriptException e1) {
                    score += 3;
                    try {
                        calculator.eval("a=1/0;");//除0
                        failed(name);
                    } catch (ScriptException e2) {
                        score += 4;
                        pass(name);
                    }
                }
            }
        } else {
            failed(name);
        }
    }

    private static void pass(String name) {
        System.out.printf("%-32s:", name);
        System.out.println("pass");
    }

    private static void failed(String name) {
        System.out.printf("%-32s:", name);
        System.out.println("fail");
    }
}
