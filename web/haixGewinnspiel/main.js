const nachname = "";
const vorname = "";
const strasse = "";
const hausnummer = "";
const plz = "";
const ort = "";
const land = "";
const email = "";


function getMsLeft() {
  var now = new Date();
  var then = new Date();
  then.setHours(10, 0, 0, 0);
  return (then - now);
}

function orderNow() {
  console.log("GO! GO! GO!");

  var fields = document.getElementsByClassName("wpcf7-form-control wpcf7-text wpcf7-validates-as-required");
  for (var i = 0; i < fields.length; i++) {
    var field = fields.item(i);
    switch (field.name) {
      case "nachname":
        field.value = nachname;
        break;
      case "vorname":
        field.value = vorname;
        break;
      case "strasse":
        field.value = strasse;
        break;
      case "hausnummer":
        field.value = hausnummer;
        break;
      case "plz":
        field.value = plz;
        break;
      case "ort":
        field.value = ort;
        break;
      case "email":
        field.value = email;
        break;
      default:
        console.log("Field without case: " + field.name);
    }
  }

  for (var i = 0; i < 2; i++) {
    document.getElementsByClassName("wpcf7-list-item").item(i).children.item(0).children.item(0).checked = true;  
  }
  
  var button = document.getElementsByClassName("wpcf7-form-control wpcf7-submit btn btn-black mt-4").item(0);
  button.disabled = false;

  setTimeout(function() {
    button.click();
    console.log("Done!!!");
  }, 500);
}

function check() {
  if (getMsLeft() < 10000) { // 10 seconds left
    console.log("Checking for availability");

    var elements = document.getElementsByClassName("modal-toggle btn");
    if (elements.length == 0) {
      console.log("Reload triggerd");
      location.reload();
    } else {
      item(0).click();
      setTimeout(orderNow, 200);
    }
    return;
  }

  console.log("Still " + Math.round(getMsLeft() / 6e4) + " Minutes to go...");
}

function main() {

  console.log("Haix winner loaded");

  if (window.location != "https://www.fireweeks.de/#") {
    console.log("Unknown page..");
    return;
  }

  check();
  setInterval(function() {
    check();
  }, 5 * 1000);
}

main();