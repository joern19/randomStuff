function getMsLeft() {
  var now = new Date();
  var then = new Date();
  then.setHours(10, 0, 0, 0);
  return (then - now);
}

function orderNow() { // todo
  console.log("GO! GO! GO!");
}

function check() {
  if (getMsLeft() < 10000) { // 10 seconds left
    console.log("Checking for availability");

    if (document.getElementsByClassName("btn btn-disabled px-4").item(0).children.item(1).firstChild.data == "Morgen erneut dein Glück versuchen") {
      console.log("Reload triggerd");
      location.reload();
    } else {
      orderNow();
    }
    return;
  }

  console.log("Still " + Math.round(getMsLeft() / 6e4) + " Minutes to go...");
}

function main() {

  console.log("Haix winner loaded");

  if (window.location != "https://www.fireweeks.de/") {
    console.log("Unknown page..");
    return;
  }

  check();
  setInterval(function() {
    check();
  }, 5 * 1000);
}

main();