<html>
  <head>
    <link rel="stylesheet" type="text/css" href="style.css">
    <script> var DEBUG=false ; var ws; var state;var data_rate=0; var trigger_rate=0; var RunNumber;</script>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <script type="text/javascript" src="./jsroot/scripts/JSRootCore.js?gui"></script>
    <script src="./gauge.min.js"></script>
    <title>DAQ Control</title>

    <script type = "text/javascript">
      function StartWebSocket() 
      {
        if ("WebSocket" in window) 
        {
          // Let us open a web socket
          ws = new WebSocket("ws://127.0.0.1:8080");
 
          ws.onopen = function() 
          {
            document.body.style.background = "#b3e0ff";
            var element = document.getElementById('EventNumber');
            element.innerHTML = "<h1>Event Number : -- </h1>";
            document.getElementById("NotConnected").style.display='none';
            document.getElementById("Actions").style.display='block';
            document.getElementById('Initialize').disabled=true;
            document.getElementById('Configure').disabled=true;
            document.getElementById('Connect').disabled =true;
            document.getElementById('Start').disabled =true;
            document.getElementById('Pause').disabled =true;
            document.getElementById('Stop').disabled =true;
            document.getElementById('Disconnect').disabled =true;
            document.getElementById('Clear').disabled  =true;
            document.getElementById('Release').disabled =true;
            var myDiv = document.getElementById('Channels');
            for(var i=0;i<=32;++i)
            {
              var checkbox = document.createElement('input');
              checkbox.type = "checkbox";
              checkbox.name = "BoxChannel"+i;
              checkbox.id = "BoxChannel"+i;
              checkbox.for="Channel"+i;
              checkbox.onclick = function() 
              {
                // access properties using this keyword
                if ( this.checked ) 
                {
                  // if checked ...
                  document.getElementById(this.for).style.display='block';
                } 
                else 
                {
                  // if checked ...
                  document.getElementById(this.for).style.display='none';
                }
              };
              myDiv.appendChild(checkbox);
            }
          };
          
          ws.onmessage = function (evt) 
          { 
            var obj = JSON.parse(evt.data)
            if(DEBUG==true) alert(evt.data);
            if(obj.Type=="State") ParseState(obj);
            if(obj.Type=="Info") ParseInfo(obj);
          };
          
          ws.onclose = function() 
          { 
            document.getElementById("Actions").style.display='none';
          };
        } 
        else 
        {  
          // The browser doesn't support WebSocket
          alert("WebSocket NOT supported by your Browser!");
        }
      }
    </script>
    
    <!-- Execute when the page is opening -->
    <script>
    	window.onload=function()
		{
			JSROOT.AssertPrerequisites("2d,histo");
			myFunction();
			document.getElementById("Actions").style.display='none';
			document.body.style.background = "red";
			StartWebSocket();
		}
	</script>
	<!-- Execute when the page is closed -->
	<script>
		window.onclose=function()
		{
			ws.close();
		}
	</script>
	<!-- Function for the websocket -->
	<script>
		function Action(...args)
		{
			var obj = new Object();
			obj.Content = arguments[0].toString();
			send(JSON.stringify(obj));
		}

		function Infos(...args)
		{
			var obj = new Object();
			obj.Type = "Info";
			obj.Content=arguments[0].toString();
    		for(var arg = 1; arg < arguments.length; ++ arg)
			{
				obj[arguments[arg].toString()]=arguments[arg+1].toString();
				++arg;
			}
			send(JSON.stringify(obj));
		}

		function Action(...args) 
		{
			var obj = new Object();
			obj.Type = "Action";
			obj.Content=arguments[0].toString();
			for(var arg = 1; arg < arguments.length; ++ arg)
			{
				obj[arguments[arg].toString()]=arguments[arg+1].toString();
				++arg;
			}
			send(JSON.stringify(obj));
		}

		function send(command) 
		{
			if(DEBUG==true)alert(command);
			ws.send(command);
		}
		
		function ParseState(obj)
		{
			if (obj["Content"]=="UNINITIALIZED")
			{
				document.getElementById('Initialize').disabled=false;
				document.getElementById('Connect').disabled =true;
				document.getElementById('Configure').disabled=true;
				document.getElementById('Start').disabled =true;
				document.getElementById('Pause').disabled =true;
				document.getElementById('Stop').disabled =true;
				document.getElementById('Clear').disabled  =true;
				document.getElementById('Disconnect').disabled =true;
				document.getElementById('Release').disabled =true;
			}
			else if (obj["Content"]=="INITIALIZED")
			{
				document.getElementById('Initialize').disabled=true;
				document.getElementById('Connect').disabled =false;
  				document.getElementById('Configure').disabled =true;
    			document.getElementById('Start').disabled =true;
    			document.getElementById('Pause').disabled =true;
    			document.getElementById('Stop').disabled =true;
    			document.getElementById('Clear').disabled  =true;
    			document.getElementById('Disconnect').disabled =true;
    			document.getElementById('Release').disabled =false;
			}
			else if(obj["Content"]=="CONNECTED")
			{
				document.getElementById('Initialize').disabled=true;
				document.getElementById('Connect').disabled =true;
  				document.getElementById('Configure').disabled =false;
    			document.getElementById('Start').disabled =true;
    			document.getElementById('Pause').disabled =true;
    			document.getElementById('Stop').disabled =true;
    			document.getElementById('Clear').disabled  =true
    			document.getElementById('Disconnect').disabled =false;
    			document.getElementById('Release').disabled =true;
			}
			else if(obj["Content"]=="CONFIGURED")
			{
				document.getElementById('Initialize').disabled=true;
				document.getElementById('Connect').disabled =true;
  				document.getElementById('Configure').disabled =true;
    			document.getElementById('Start').disabled =false;
    			document.getElementById('Pause').disabled =true;
    			document.getElementById('Stop').disabled =true;
    			document.getElementById('Clear').disabled  =false;
    			document.getElementById('Disconnect').disabled =true;
    			document.getElementById('Release').disabled =true;
			}
			else if(obj["Content"]=="STARTED")
			{
  				document.getElementById('Initialize').disabled=true;
  				document.getElementById('Connect').disabled =true;
  				document.getElementById('Configure').disabled =true;
    			document.getElementById('Start').disabled =true;
    			document.getElementById('Pause').disabled =false;
    			document.getElementById('Stop').disabled =false;
    			document.getElementById('Clear').disabled  =true;
    			document.getElementById('Disconnect').disabled =true;
    			document.getElementById('Release').disabled =true;
			}
			else if(obj["Content"]=="PAUSED")
			{
  				document.getElementById('Initialize').disabled=true;
  				document.getElementById('Connect').disabled =true;
  				document.getElementById('Configure').disabled =true;
    			document.getElementById('Start').disabled =false;
    			document.getElementById('Pause').disabled =true;
    			document.getElementById('Stop').disabled =false;
    			document.getElementById('Clear').disabled  =true;
    			document.getElementById('Disconnect').disabled =true;
    			document.getElementById('Release').disabled =true;				
			}
			else  if(obj["Content"]=="STOPED")
			{
  				document.getElementById('Initialize').disabled=true;
  				document.getElementById('Connect').disabled =true;
  				document.getElementById('Configure').disabled =true;
    			document.getElementById('Start').disabled =false;
    			document.getElementById('Pause').disabled =true;
    			document.getElementById('Stop').disabled =true;
    			document.getElementById('Clear').disabled  =false;
    			document.getElementById('Disconnect').disabled =true;
    			document.getElementById('Release').disabled =true;
			}
			else if(obj["Content"]=="CLEARED")
			{
  				document.getElementById('Initialize').disabled=true;
  				document.getElementById('Connect').disabled =true;
  				document.getElementById('Configure').disabled =false;
    			document.getElementById('Start').disabled =true;
    			document.getElementById('Pause').disabled =true;
    			document.getElementById('Stop').disabled =true;
    			document.getElementById('Clear').disabled  =true;
    			document.getElementById('Disconnect').disabled =false;
    			document.getElementById('Release').disabled =true;
  			}
			else if(obj["Content"]=="DISCONNECTED")
			{
  				document.getElementById('Initialize').disabled=true;
  				document.getElementById('Connect').disabled =false;
  				document.getElementById('Configure').disabled =true;
    			document.getElementById('Start').disabled =true;
    			document.getElementById('Pause').disabled =true;
    			document.getElementById('Stop').disabled =true;
    			document.getElementById('Clear').disabled  =true;
    			document.getElementById('Disconnect').disabled =true;
    			document.getElementById('Release').disabled =false;
    			document.getElementById('On').disabled =true;
				document.getElementById('Off').disabled=true;
			}
			else if(obj["Content"]=="RELEASED")
			{
				document.getElementById('Initialize').disabled=false;
				document.getElementById('Connect').disabled =true;
				document.getElementById('Configure').disabled=true;
				document.getElementById('Start').disabled =true;
				document.getElementById('Pause').disabled =true;
				document.getElementById('Stop').disabled =true;
				document.getElementById('Clear').disabled  =true;
				document.getElementById('Disconnect').disabled =true;
				document.getElementById('Release').disabled =true;
			}
		}
		function ParseInfo(obj)
		{
			if(obj["Name"]=="EventNbr")
			{
				var element = document.getElementById('EventNumber');
				element.innerHTML = obj["EventNbr"];
				element.innerHTML = "<h1>Event Number : "+obj["EventNbr"]+"</h1>";
			}
			else if(obj["Name"]=="Plot")
			{
				if(DEBUG==true)alert(obj["Plot"]);
				var obj2 = JSROOT.parse(obj["Plot"]);
  				var ObjectName=obj2.fName;
  				if(document.getElementById(obj2.fName).style.display=="block"||obj2.fName=="Trigger0"||obj2.fName=="Trigger1")
  				{
  					obj2.fYaxis.fTitle="ADC Counts";
  					//obj2.fYaxis.fXmax=1.0;
  					//obj2.fYaxis.fXmin=0.0;
  					//obj2.fMaximum=0.7;
  					//obj2.fMinimum=0.6;
    				JSROOT.redraw(ObjectName, obj2,"histo");
  				}
			}
			else if(obj["Name"]=="RunNumber")
			{
				RunNumber = obj["RunNumber"];
			}
			else if(obj["Name"]=="DataRate")
			{
				old_data_rate=data_rate;
				data_rate=Number(obj["DataRate"]);
	    		var gauge = new RadialGauge({
    			renderTo: 'DataRate',
    			width: 150,
    			height: 150,
    			value : data_rate ,
    			units: "MB/s",
    			minValue: 0,
    			maxValue: 5,
    			majorTicks: ["0.1","0.2","0.3","0.4","0.5","0.6","0.7","0.8","0.9","1.0","5.0"],
    			minorTicks: 9,
    			strokeTicks: true,
    			highlights: [
        		{
            		"from": 0,
            		"to": 0,
            		"color": "grey"
        		}
    			],
    			colorPlate: "#fff",
    			borderShadowWidth: 0,
    			borders: true,
    			needleType: "arrow",
    			needleWidth: 2,
    			needleCircleSize: 7,
    			needleCircleOuter: true,
    			needleCircleInner: false,
    				animationDuration: 800,
    			animationRule: function(percent) {
    				if(percent<=0) return 0;
        return 1 - (function(p) {
            var x = 0.8;
            return Math.pow(2, 10 * (p - 1)) * 
                   Math.cos(20 * Math.PI * x / 3 * p);
        })(1 - percent);
    },
    			animateOnInit: true,
    			animatedValue: true,
				}).draw();
			}
			else if(obj["Name"]=="TriggerRate")
			{
				old_trigger_rate=trigger_rate;
				trigger_rate=Number(obj["TriggerRate"]);
				var gauge = new RadialGauge({
    			renderTo: 'TriggerRate',
    			width: 150,
    			height: 150,
    			value : trigger_rate ,
    			units: "Hz",
    			minValue: 0,
    			maxValue: 20,
    			majorTicks: ["1","2","3","4","5","6","7","8","9","10"],
    			minorTicks: 9,
    			strokeTicks: true,
    			highlights: [
        		{
            		"from": 0,
            		"to": 0,
            		"color": "grey"
        		}
    			],
    			colorPlate: "#fff",
    			borderShadowWidth: 0,
    			borders: true,
    			needleType: "arrow",
    			needleWidth: 2,
    			needleCircleSize: 7,
    			needleCircleOuter: true,
    			needleCircleInner: false,
    			animationDuration: 800,
    			animationRule: function(percent) {
    				if(percent<=0) return 0;
        return 1 - (function(p) {
            var x = 0.8;
            return Math.pow(2, 10 * (p - 1)) * 
                   Math.cos(20 * Math.PI * x / 3 * p);
        })(1 - percent);
    },
    			animateOnInit: true,
    			animatedValue: true,
				}).draw();
			}
		}
		
	</script>
</head>


<body>

  <div id="NotConnected">
    <center>
      <h1>WebSocket Not Connected !!!</h1>
    </center>
  </div>
        
  <center>
    <div id="Actions" class="Action_buttons">
      <button id="Initialize" onclick='Action("INITIALIZE");'>Initialize</button>
      <button id="Connect" onclick='Action("CONNECT");'>Connect</button>
      <button id="Configure" onclick='Action("CONFIGURE");'>Configure</button>
      <button id="Start" onclick='openForm("START");'>Start</button>
      <button id="Pause" onclick='Action("PAUSE");'>Pause</button>
      <button id="Stop" onclick='Action("STOP");'>Stop</button>
      <button id="Clear" onclick='Action("CLEAR");'>Clear</button>
      <button id="Disconnect" onclick='Action("DISCONNECT");'>Disconnect</button>
      <button id="Release" onclick='Action("RELEASE");'>Release</button>
    </div>
  </center>
 
  <center>
    <form id="Channels"></form>
  </center>

  <center>
    <div style="display:flex; flex-direction: row; justify-content: center; align-items: center">
      <canvas id="DataRate" ></canvas>
      <div id="EventNumber"></div> 
      <canvas id="TriggerRate" ></canvas>
    </div>
  </center>
 
  <!-- Run info form -->
  <div class="form-popup" id="myForm">
    <form action="#" class="form-container">
      <center id="RunInfos">Run Infos</center>
      <label for="Comment"><b>Comment</b></label>
      <input type="text" placeholder="Enter Comment" id="Comment" name="Comment">
      <button type="submit" class="btn" onclick="startAquisition(this.form)">Start</button>
      <button type="button" class="btn cancel" onclick="closeForm()">Close</button>
    </form>
  </div>
 
  <!-- Script for the form -->
  <script>
    function openForm() 
    {
      document.getElementById('RunInfos').innerHTML = "<h1>Run "+RunNumber+" Infos</h1>";
      if(state=="PAUSED")Action("START");
      else document.getElementById("myForm").style.display = "block";
    }

    function startAquisition(oForm)
    {
      //Info("RunInfos ="+oForm.elements["Comment"].value);
      Action("START");
    }
    function closeForm() 
    {
      document.getElementById("myForm").style.display = "none";
    }
  </script>

  <center>
    <script>
      function myFunction()
      {
        var myDiv = document.getElementById("Plots")
        for(var i = 0; i < 36; i++)
        {
          var t = document.createElement("div");
          t.setAttribute("id", "Channel"+i);
          t.setAttribute("what", "plot");
          t.style.display='none';
          myDiv.appendChild(t);
        }
       }
     </script>
 
    <div id="Plots">
      <input type="checkbox" class="test" id="btnControltr1"/>
      <label class="btn" for="btnControltr1"><div id="Trigger0" what="plot" ></div></label>
      <label class="btn" for="btnControltr1"><div id="Trigger1" what="plot" ></div></label>
    </div>
   </center>
  </body>
</html>
