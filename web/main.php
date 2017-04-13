<!DOCTYPE html>
<html>
<head><title>PlantAction</title></head>  
<body>
 <!--Header-->
 <div id="page-content-wrapper"><img  width="1000px"  src="http://localhost:8000/images/header.png" id="logo"></div>    
 <a style="font-family: Verdana">PlantAction Dashboard</a>    
    <p style="font-family: Verdana">System ID: 00001</p>
    
 
 <h2 style="font-family: Verdana">Spray!</h2>

 <label class="switch"><input type="checkbox" id="cbox"><div class="slider"></div></label>

<h2 style="font-family: Verdana">Data Monitoring:</h2>
<div id=graph>
<iframe width="450" height="250" style="border: 1px solid #cccccc;" src="http://thingspeak.com/channels/184808/charts/1?dynamic=true"></iframe>

<!--Optional temperature and air humidity readings-->
<!--
<iframe width="450" height="250" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/184808/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Temperature&type=line&yaxis=Degrees+Celcius"></iframe>

<iframe width="450" height="250" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/184808/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Humidity&type=line"></iframe>
</div>   
-->
<!-- CSS here -->    
<style>
    html, body {
    margin: 100;
    padding: 100;
}
   .switch {
  position: relative;
  display: inline-block;
  width: 60px;
  height: 34px;
}

/* Hide default HTML checkbox */
.switch input {display:none;}

/* The slider */
.slider {
  position: absolute;
  cursor: pointer;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: #ccc;
  -webkit-transition: .4s;
  transition: .4s;
}

.slider:before {
  position: absolute;
  content: "";
  height: 26px;
  width: 26px;
  left: 4px;
  bottom: 4px;
  background-color: white;
  -webkit-transition: .4s;
  transition: .4s;
}

input:checked + .slider {
  background-color: #2196F3;
}

input:focus + .slider {
  box-shadow: 0 0 1px #2196F3;
}

input:checked + .slider:before {
  -webkit-transform: translateX(26px);
  -ms-transform: translateX(26px);
  transform: translateX(26px);
}
    
    .button input {display:none;}
    #button-spray{
        width: 80;
    }
    #unclick{
        content:url("http://localhost:8000/images/button.png")
    }
    #unclick.toggled{
        content:url("http://localhost:8000/images/button-click.png");
    }
</style>    

    
<!-- JavaScript here -->
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
<script>
    
$("#cbox").change(function () {
    var cmd = this.checked ? "TURN_ON" : "TURN OFF";
    $.ajax({
        url: "https://api.thingspeak.com/talkbacks/11796/commands/3224092",
        type: 'PUT',
        data: {api_key: "yourapikey",command_string:cmd},
        success: function(result, data, status) {
            alert('Update success: '+result);
            $.get("https://api.thingspeak.com/talkbacks/11796/commands/3224092?api_key=yourapikey", function(data, status){
        alert("Data: " + data + "\nStatus: " + status);
        });
    }});
    });   

</script>
</body>    
</html>