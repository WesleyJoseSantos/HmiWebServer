function communicationStart(interval) {
    setInterval(function ( ) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            var data = JSON.parse(this.responseText);
            console.log(this.responseText);
            if(data != null){
              // for (const key in data) {
              //   if (Object.hasOwnProperty.call(data, key)) {
              //     const value = data[key];
              //     updateObject(key, value);
              //   }
              // }
              data.forEach(tag => {
                updateObject(tag.Name, tag.Value);
              });
            }
          }
        };
        xhttp.open("GET", "/?tags=true", true);
        xhttp.send();
    }, interval) ;
}

function updateObject(id, value){
    var obj = document.getElementById(id);
    if(obj == null) return;
    if(obj.classList.contains("sti")){
        obj.className = value ? "sti on" : "sti off";
    }
    if(obj.classList.contains("str")){
        obj.className = value ? "str on" : "str err";
    }
    if(obj.classList.contains("value-display")){
        var label = obj.innerText.split(":")[0] + ": ";
        obj.innerText = label + value;
    }
    if(obj.classList.contains("w3-container")){
        var width = value * 0.4;
        obj.innerText = value;
        obj.style = "width:" + width + "%; display: inline-block";
    }
    if(obj.classList.contains("circle")){
        obj = new RadialProgress(document.getElementById(id));
        if(obj!=null){
            obj.setValue(Math.abs(value / 100));
        }
    }
}