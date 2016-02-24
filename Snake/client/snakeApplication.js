var snakeApplication = function(){
	//Canvas stuff
	var canvas = $("#canvas")[0];
	var ctx = canvas.getContext("2d");
	var w = $("#canvas").width();
	var h = $("#canvas").height();
	
	//Lets save the cell width in a variable for easy control
	var cw = 10;
	var d;
	var d2;
	var food;
	var score;
	var score2;
	var Player1Name = "Player1";
	var Player2Name = "Player2";

	//Lets create the snake now
	var snake_array; //an array of cells to make up the snake
	var snake_array2;
	

	this.init = function()
	{
		d = "down"; //default direction
		d2 = "down";
		this.create_snake();
		this.create_snake2();
		//create_food(); //Now we can see the food particle
		//window.alert("food location" + food.x + " <-- x and y --> " + food.y);
		//finally lets display the score
		score = 0;
		score2 = 0;
		
		//Lets move the snake now using a timer which will trigger the paint function
		//every 60ms
		//if(typeof game_loop != "undefined") clearInterval(game_loop);
		//game_loop = setInterval(this.paint, 200);
	}

	this.create_snake = function()
	{
		var length = 5; //Length of the snake
		snake_array = []; //Empty array to start with
		for(var i = length-1; i>=0; i--)
		{
			//This will create a horizontal snake starting from the top left
			snake_array.push({x: 10 , y:i});
		}
	}

	this.create_snake2 = function()
	{
		var length = 5; //Length of the snake
		snake_array2 = []; //Empty array to start with
		for(var i = length-1; i>=0; i--)
		{
			//This will create a horizontal snake starting from the top left
			snake_array2.push({x: 30, y:i});
		}
	}
	
	//Lets create the food now
	function create_food()
	{
		food = {
			x: Math.round(Math.random()*(w-cw)/cw), 
			y: Math.round(Math.random()*(h-cw)/cw), 
		};
		//This will create a cell with x/y between 0-44
		//Because there are 45(450/10) positions accross the rows and columns
	}
	
	this.setFoodLocation = function(x, y)
	{
		food = {x,y};
	}

	this.setSnake1Direction = function(direction)
	{
		d = direction;
	}
	
	this.setSnake2Direction = function(direction)
	{
		d2 = direction;
	}

	this.getSnake1Direction = function()
	{
		return d;
	}
	this.getSnake2Direction = function()
	{
		return d2;
	}
	this.setPlayer1Score = function(new_score)
	{
		score = new_score;
	}

	this.setPlayer2Score = function(new_score)
	{
		score2 = new_score;
	}
	this.setScore = function(player1Score, player2Score)
	{
		score = player1Score;
		score2 = player2Score;
	}

	this.endGame = function()
	{

	}
	this.setPlayer1Name = function(name)
	{
		Player1Name = name;
	}
	this.setPlayer2Name = function(name)
	{
		Player2Name = name;
	}
	this.getWidth = function()
	{
		return w;
	}
	this.getCellWidth = function()
	{
		return cw;
	}
	this.getHeight = function()
	{
		return h;
	}
		this.addPicture = function()
	{
          var img = document.createElement("img");
          img.src = "game_over.jpg"; 
          // image sourced from entertainmentlawbrazil.com.br
          img.height = 450; 
          img.width = 450;

          //optionally set a css class on the image
          //var class_name = "foo";
          //img.setAttribute("class", class_name);

          ctx.drawImage(img, 0, 0)
	}

	this.removePicture = function()
	{
		img = null;
		ctx.removeImage(img);
	}


	//Lets paint the snake now
	this.paint = function()
	{
		//To avoid the snake trail we need to paint the BG on every frame
		//Lets paint the canvas now
		ctx.fillStyle = "white";
		ctx.fillRect(0, 0, w, h);
		ctx.strokeStyle = "black";
		ctx.strokeRect(0, 0, w, h);
		
		
		//The movement code for the snake to come here.
		//The logic is simple
		//Pop out the tail cell and place it infront of the head cell
		var nx = snake_array[0].x;
		var ny = snake_array[0].y;

		var nx2 = snake_array2[0].x;
		var ny2 = snake_array2[0].y;
		//These were the position of the head cell.
		//We will increment it to get the new head position
		//Lets add proper direction based movement now
		if(d == "right") nx++;
		else if(d == "left") nx--;
		else if(d == "up") ny--;
		else if(d == "down") ny++;

		if(d2 == "right") nx2++;
		else if(d2 == "left") nx2--;
		else if(d2 == "up") ny2--;
		else if(d2 == "down") ny2++;
		/*
		//Lets add the game over clauses now
		//This will restart the game if the snake hits the wall
		//Lets add the code for body collision
		//Now if the head of the snake bumps into its body, the game will restart
		if(nx == -1 || nx == w/cw || ny == -1 || ny == h/cw || check_collision(nx, ny, snake_array))
		{
			//restart game
			this.init();
			//Lets organize the code a bit now.
			return;
		}
		if(nx2 == -1 || nx2 == w/cw || ny2 == -1 || ny2 == h/cw || check_collision(nx2, ny2, snake_array2))
		{
			//restart game
			this.init();
			//Lets organize the code a bit now.
			return;
		}
		if(check_collision_array(snake_array, snake_array2))
		{
			this.init();
			return;
		}
		*/
		//Lets write the code to make the snake eat the food
		//The logic is simple
		//If the new head position matches with that of the food,
		//Create a new head instead of moving the tail
		if(nx == food.x && ny == food.y)
		{
			var tail = {x: nx, y: ny};
			//score++;
			//Create new food
			//create_food();
		}
		else
		{
			var tail = snake_array.pop(); //pops out the last cell
			tail.x = nx; tail.y = ny;
		}
		//The snake can now eat the food.

		if(nx2 == food.x && ny2 == food.y)
		{
			var tail2 = {x: nx2, y: ny2};
			//score2++;
			//Create new food
			//create_food();
		}
		else
		{
			var tail2 = snake_array2.pop(); //pops out the last cell
			tail2.x = nx2; tail2.y = ny2;
		}
		
		snake_array.unshift(tail); //puts back the tail as the first cell
		snake_array2.unshift(tail2);
		
		for(var i = 0; i < snake_array.length; i++)
		{
			var c = snake_array[i];
			//Lets paint 10px wide cells
			paint_cell_blue(c.x, c.y);
		}

		for(var i = 0; i < snake_array2.length; i++)
		{
			var c = snake_array2[i];
			//Lets paint 10px wide cells
			paint_cell_red(c.x, c.y);
		}

		//Lets paint the food
		paint_cell_black(food.x, food.y);
		//Lets paint the score
		var score_text =  Player1Name + " Score: " + score;
		var score_text2 = Player2Name + " Score: " + score2;
		ctx.fillStyle = "black";
		ctx.fillText(score_text, 5, h-5);
		ctx.fillText(score_text2, 100, h-5);
	}
	
	//Lets first create a generic function to paint cells
	function paint_cell_blue (x, y)
	{
		ctx.fillStyle = "blue";
		ctx.fillRect(x*cw, y*cw, cw, cw);
		ctx.strokeStyle = "white";
		ctx.strokeRect(x*cw, y*cw, cw, cw);
	}
	function paint_cell_red  (x, y)
	{
		ctx.fillStyle = "red";
		ctx.fillRect(x*cw, y*cw, cw, cw);
		ctx.strokeStyle = "white";
		ctx.strokeRect(x*cw, y*cw, cw, cw);
	}
	function paint_cell_black (x, y)
	{
		ctx.fillStyle = "black";
		ctx.fillRect(x*cw, y*cw, cw, cw);
		ctx.strokeStyle = "white";
		ctx.strokeRect(x*cw, y*cw, cw, cw);
	}
	function check_collision(x, y, array)
	{
		//This function will check if the provided x/y coordinates exist
		//in an array of cells or not
		for(var i = 0; i < array.length; i++)
		{
			if(array[i].x == x && array[i].y == y)
			{
				return true;
			}
		}
		return false;
	}

	function check_collision_array (array, array2)
	{
		for(var i = 0; i < array.length; i++)
		{
			for(var k = 0; k < array2.length; k++)
			{
				if(array[i].x == array2[k].x  && array[i].y == array2[k].y)
				{
					return true;
				}
			}	
		}
		return false;
	}
	
/*
	//Lets add the keyboard controls now
	$(document).keydown(function(e){
		var key = e.which;
		//We will add another clause to prevent reverse gear
		if(key == "37" && d != "right") d = "left";
		else if(key == "38" && d != "down") d = "up";
		else if(key == "39" && d != "left") d = "right";
		else if(key == "40" && d != "up") d = "down";
		//The snake is now keyboard controllable

		if(key == "65" && d2 != "right") d2 = "left";
		else if(key == "87" && d2 != "down") d2 = "up";
		else if(key == "68" && d2 != "left") d2 = "right";
		else if(key == "83" && d2 != "up") d2 = "down";
	})
*/
	
}