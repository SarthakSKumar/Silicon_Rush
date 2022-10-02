
  var firebaseConfig = {
    apiKey: "AIzaSyC8F66j-B81i2xQCr84FTzv5_1JcU1hMJg",
    authDomain: "shakthi-f143a.firebaseapp.com",
    databaseURL: "https://shakthi-f143a-default-rtdb.firebaseio.com",
    projectId: "shakthi-f143a",
    storageBucket: "shakthi-f143a.appspot.com",
    messagingSenderId: "1060237209326",
    appId: "1:1060237209326:web:e53f9a3aae8909e5b6cdff",
    measurementId: "G-BCXDWXZ6CV"
  };
  
firebase.initializeApp(firebaseConfig);


$(document).ready(function(){
    var database = firebase.database();
	var Motor1Status;
	var ref = firebase.database().ref();
	ref.on("value", function(snapshot) {
		console.log(snapshot.val());
	 }, function (error) {
		console.log("Error: " + error.code);
	 });

	database.ref().on("value", function(snap){
		Motor1Status = snap.val().Motor1Status;
		if(Motor1Status == "1"){    // check from the firebase
			//$(".Light1Status").text("The light is off");
			document.getElementById("unact").style.display = "none";
			document.getElementById("act").style.display = "block";
		} else {
			//$(".Light1Status").text("The light is on");
			document.getElementById("unact").style.display = "block";
			document.getElementById("act").style.display = "none";
		}
	});

    $(".toggle-btn").click(function(){
		var firebaseRef = firebase.database().ref().child("Motor1Status");

		if(Motor1Status == "1"){    // post to firebase
			firebaseRef.set("0");
			Motor1Status = "0";
		} else {
			firebaseRef.set("1");
			Motor1Status = "1";
		}
	})
});