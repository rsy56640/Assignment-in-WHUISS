<!DOCTYPE html>
<html>
<head>rsy56640</head>
<meta charset="utf-8">
<title>myPage</title>

<body>

<%@ page import="java.util.*" %>
<%@ page import="java.lang.*" %>
<%@ page session="true" %>

<%!
public int foo()
{
	return 0;
}
%>
<%! int x=foo(); %>
<% out.println(x); %>


<h1>This is a game of WuZiQi</h1>
<p id="demo">Here is a look</p>

<button type="button" onclick="displayDate()">show date</button>

<!--

<button type="button" onclick="goo()">show num</button>

-->


<!--
<div id="container"   style="width:400px;height:400px;border:1px solid #000000;margin-left:150px">
		<canvas 
			id="myCanvas" width="0" height="400"  
            style="border:1px solid #000000;margin-left:20px">  
        </canvas> 
		<canvas 
			id="myCanvas" width="0" height="400"  
            style="border:1px solid #000000;margin-left:20px">  
        </canvas> 
		<canvas 
			id="myCanvas" width="0" height="400"  
            style="border:1px solid #000000;margin-left:20px">  
        </canvas> 
		<canvas 
			id="myCanvas" width="0" height="400"  
            style="border:1px solid #000000;margin-left:20px">  
        </canvas> 
</div>    
-->


<%! String img_black=new String("./balck.png"); %>
<%! String img_white=new String("./white.png"); %>


<img src="./black.png"> </img>
<img src="./white.png"> </img>


<input type = "text" id = "Xaxis" name = "X" value = "0">
<input type = "text" id = "Yaxis" name = "Y" value = "0">

<button type="button" onclick="getXY(this)">submit</button>

<br><br><br>

<canvas id="mycanvas" width="500" height="500" style="border:1px solid #000;margin-left:100px"></canvas>

</body>


<script>


function displayDate()
{
	document.getElementById("demo").innerHTML=Date();
}

var times=0;

function goo()
{
	
	document.getElementById("demo").innerHTML = new String(++times);
}

	
		
/*
function drawTable()
{
	String str="";
        for(int i=0;i<19;++i)
        {
            str=str	+ "<canvas"
                    + 	"  id=\"myCanvas\" width=\"0\" height=\"400\""
                    + 	"  style=\"border:1px solid #000000;margin-left:20px\">  "
                    + "</canvas>\n";
        }
	
	document.getElementById("container").innerHTML =
		new String
	(
		"<div id=\"container\" style=\"width:400px;height:400px;border:1px solid #000000;margin-left:150px\">\n  "
	+	str
	+ 	"</div>\n" 
	);

}
*/



/*
function getMousePos(event) 
{
    var e = event || window.event;
    return {'x':e.screenX,'y':screenY}；
}
*/

var X;
var Y;
		
var x;
var y;



     // 1. 获取标签
    var canvas = document.getElementById("mycanvas");
    var ctx = canvas.getContext("2d");

    // 2.绘制表格
    // 2.1 常量
    var marginXY = 25;
    var rows = canvas.height / marginXY,
        cols = canvas.width / marginXY;
    // 2.2 绘制横线
    for (var i=1; i<rows; i++){
        // 起点
        ctx.moveTo(0, i*marginXY);
        // 终点
        ctx.lineTo(canvas.width, i*marginXY);
    }
	
	ctx.strokeStyle='deepblack';
	
    // 2.3 绘制竖线
    for (var i=1; i<cols; i++){
         // 起点
         ctx.moveTo(i*marginXY, 0);
         // 终点
         ctx.lineTo(i*marginXY, canvas.height);
    }

    // 绘制
    ctx.strokeStyle = 'deepblack';
    ctx.lineWidth = .5;
    ctx.stroke();

	
	
	

	var isEnd=false;
	var winner=0;							//black=1, white=2.
	var img_black="./black.png";
	var img_white="./white.png";
	var imgs = new Array(2);
	imgs[1] = img_black;
	imgs[0] = img_white;
	var black_turn=true;
	var length=21;
	var num=0;
	

	var matrix = new Array(29);
	
    for(var i=0;i<29;i++)
	{
        matrix[i] = new Array(29);
        for(var j=0;j<29;j++)
		{
            matrix[i][j] = 0;				//black=1, white=2.
        }
    }
	

	
	document.write("Hello World!");
	
	document.write("<img src="+ imgs[1] +"  > </img>");
	
	
	
function getXY(event)
{
	//alert(event.offsetX);
	if(isEnd)
	{
		var tmp= new String("");
		if(winner==1)tmp=tmp+"black";
		else tmp=tmp+"white";
		document.getElementById("demo").innerHTML="The winner is "+tmp;
		return;
	}
	
	X=document.getElementById("Xaxis").value;
	Y=document.getElementById("Yaxis").value;
	
	X=parseInt(X);
	Y=parseInt(Y);
	
	if(X<1 || X>21 || Y<1 || Y>21)
	{
		var tmp="invalid position";
		document.getElementById("demo").innerHTML=tmp;
		return;
	}
	var XX=3+parseInt(X);
	var YY=3+parseInt(Y);
	var str= new String("");
	if(matrix[XX][YY]!=0)str=str+" repetition."
	else if(black_turn)str=str+"    black turn.";
	else str=str+"    white turn.";
	document.getElementById("demo").innerHTML="X= "+X+", Y= "+Y+str;
	if(matrix[XX][YY]!=0)return;
	
	
	/*
		var mouseX;
		var mouseY;
		
		canvas.onmousedown = 
			function(evt) {  
				evt = window.event || evt;        
				//获取canvas相对于浏览器圆点的坐标  
				var rect = canvas.getBoundingClientRect();  
				//获取圆心的位置         
				//获取鼠标在canvas上的位置  
				mouseX = (evt.pageX - rect.left) * (canvas.width / rect.width);  
				mouseY = (evt.pageY - rect.top) * (canvas.height / rect.height);  
			}  
			
			document.write (mouseX);

			*/
	
		
		var hashMap=new Object();
		
		//21*21
		var pos=
		{
			tableX: X	,
			tableY: Y	,
			color: ""	
		}
		
		num++;
		
		
		//add a image
		if(black_turn)	//black
		{
			//black_turn=false;
			pos.color="black";
			hashMap[num]=pos;
			matrix[XX][YY]=1;
			
			x = 25 * X - 36;
			y = 25 * Y - 36;
			
			var img = new Image();  
			img.src = imgs[1];
			ctx.drawImage(img, x, y);
			
			//check
			var win=false;
			
			//left-right
			for(var i=-4;i<1;++i)
			{
				var flag=true;
				for(var j=0;j<5;++j)
				{
					if(matrix[i+XX+j][YY]!=1)
					{
						flag=false;
						break;
					}
				}
				if(flag)win=true;
				if(win)break;
			}
			//up-down
			for(var i=-4;i<1;++i)
			{
				var flag=true;
				for(var j=0;j<5;++j)
				{
					if(matrix[XX][YY+i+j]!=1)
					{
						flag=false;
						break;
					}
				}
				if(flag)win=true;
				if(win)break;
			}
			//left-up right-down
			for(var i=-4;i<1;++i)
			{
				var flag=true;
				for(var j=0;j<5;++j)
				{
					if(matrix[XX+i+j][YY+i+j]!=1)
					{
						flag=false;
						break;
					}
				}
				if(flag)win=true;
				if(win)break;
			}	
			//right-up left-down
			for(var i=-4;i<1;++i)
			{
				var flag=true;
				for(var j=0;j<5;++j)
				{
					if(matrix[XX+i+j][YY-i+j]!=1)
					{
						flag=false;
						break;
					}
				}
				if(flag)win=true;
				if(win)break;
			}	

			//check
			if(win)
			{
				isEnd=true;
				winner=1;
				document.getElementById("demo").innerHTML="The winner is black";
			}
			
			black_turn=false;
			
		}
		else{			//white
		
			black_turn=true;
			pos.color="white";
			hashMap[num]=pos;
			matrix[XX][YY]=2;
			
			x = 25 * X - 36;
			y = 25 * Y - 36;

			
			var img = new Image();  
			img.src = imgs[0];
			ctx.drawImage(img, x, y);

			//check
			var win=false;
			
			//left-right
			for(var i=-4;i<1;++i)
			{
				var flag=true;
				for(var j=0;j<5;++j)
				{
					if(matrix[i+XX+j][YY]!=2)
					{
						flag=false;
						break;
					}
				}
				if(flag)win=true;
				if(win)break;
			}
			//up-down
			for(var i=-4;i<1;++i)
			{
				var flag=true;
				for(var j=0;j<5;++j)
				{
					if(matrix[XX][YY+i+j]!=2)
					{
						flag=false;
						break;
					}
				}
				if(flag)win=true;
				if(win)break;
			}
			//left-up right-down
			for(var i=-4;i<1;++i)
			{
				var flag=true;
				for(var j=0;j<5;++j)
				{
					if(matrix[XX+i+j][YY+i+j]!=2)
					{
						flag=false;
						break;
					}
				}
				if(flag)win=true;
				if(win)break;
			}	
			//right-up left-down
			for(var i=-4;i<1;++i)
			{
				var flag=true;
				for(var j=0;j<5;++j)
				{
					if(matrix[XX+i+j][YY-i+j]!=2)
					{
						flag=false;
						break;
					}
				}
				if(flag)win=true;
				if(win)break;
			}	
			
			//check
			if(win)
			{
				isEnd=true;
				winner=2;
				document.getElementById("demo").innerHTML="The winner is white.";
			}
			
			black_turn=true;
			
		}//end else
		
	
}



</script>



</html>