
#pragma once
#include <avr/pgmspace.h>

const char webpage[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Dog Buzzer</title>
<style>
* { box-sizing: border-box; }
body {
  margin: 0;
  min-height: 100vh;
  background: #b5e6ff;
  font-family: Arial, sans-serif;
  color: #092342;
  text-align: center;
}
main {
  max-width: 500px;
  margin: auto;
  padding: 24px 10px;
}
.logo {
  display: block;
  width: 210px;
  height: 210px;
  margin: 0 auto 12px;
}
h1 {
  font-size: 38px;
  margin: 8px 0 28px;
}
label {
  display: block;
  font-size: 23px;
  margin-bottom: 10px;
}
select {
  width: 100%;
  padding: 14px;
  font-size: 22px;
  text-align: center;
  border: 2px solid #aaa;
  border-radius: 12px;
  background: white;
}
button {
  display: block;
  width: 100%;
  margin-top: 24px;
  padding: 22px;
  font-size: 30px;
  font-weight: bold;
  color: white;
  background: #24bd39;
  border: 3px solid #07851c;
  border-radius: 18px;
  cursor: pointer;
}
button:disabled {
  background: #87b994;
  border-color: #659575;
}
#status {
  font-size: 23px;
  margin-top: 25px;
}
</style>
</head>
<body>
<main>

<svg class="logo" viewBox="-12 -12 264 264"
     xmlns="http://www.w3.org/2000/svg"
     role="img" aria-label="No barking dog">
  <!-- White background -->
  <circle cx="120" cy="120" r="120" fill="white"/>

  <!-- Cartoon dog -->
  <g stroke="#29231e" stroke-width="3.5"
     stroke-linecap="round" stroke-linejoin="round">
    <!-- Tail -->
    <path d="M68 142 Q34 143 43 108 Q47 95 52 104
             Q47 124 72 127"
          fill="#fff4e4"/>
    <!-- Body -->
    <path d="M70 125 Q76 104 116 110 L151 130
             L151 172 L137 176 L125 153
             L99 153 L94 180 L80 180 L77 149
             Q65 148 70 125Z"
          fill="#fff8e9"/>
    <!-- Brown body patch -->
    <path d="M82 114 Q108 104 121 121
             L107 146 L88 140Z"
          fill="#c7833c"/>
    <!-- Legs -->
    <path d="M134 149 L145 185 Q153 190 160 183
             L151 145Z"
          fill="#fff8e9"/>
    <path d="M83 150 L77 185 Q87 193 96 184
             L102 151Z"
          fill="#fff8e9"/>
    <!-- Head -->
    <path d="M125 64 Q144 51 166 65
             Q181 72 181 93
             L194 105 L182 116
             L171 127 L139 120
             Q116 102 125 64Z"
          fill="#fff8e9"/>
    <!-- Ear -->
    <path d="M137 66 Q116 52 110 78
             Q112 98 128 103 L139 80Z"
          fill="#aa642f"/>
    <!-- Face patch -->
    <path d="M144 66 Q163 59 175 76
             L170 98 Q158 107 144 96Z"
          fill="#d79246"/>
    <!-- Eye -->
    <circle cx="161" cy="83" r="5" fill="#171717"/>
    <circle cx="162" cy="81" r="1.5"
            fill="white" stroke="none"/>
    <!-- Nose -->
    <ellipse cx="192" cy="103" rx="8" ry="6"
             fill="#171717"/>
    <!-- Open barking mouth -->
    <path d="M171 111 Q185 117 196 110
             Q194 137 179 133 Q169 127 171 111Z"
          fill="#331b1b"/>
    <path d="M178 128 Q184 119 191 125"
          fill="#f17e91" stroke="none"/>
    <!-- Collar -->
    <path d="M132 117 L153 129"
          stroke="#d51b1b" stroke-width="7"/>
    <circle cx="147" cy="133" r="5"
            fill="#ffd33e"/>
  </g>

  <!-- Bark sound lines -->
  <g stroke="#151515" stroke-width="5"
     stroke-linecap="round">
    <path d="M202 83 L217 72"/>
    <path d="M205 103 L224 101"/>
    <path d="M202 123 L218 135"/>
  </g>

  <!-- Prohibition symbol on top -->
  <circle cx="120" cy="120" r="120"
          fill="none" stroke="#ff1616"
          stroke-width="17"/>
  <line x1="35" y1="35" x2="205" y2="205"
        stroke="#ff1616" stroke-width="17"/>
</svg>

<h1>Dog Buzzer</h1>

<label for="duration">Duration</label>
<select id="duration">
  <option value="1" selected>Stop the Yipping (1 second)</option>
  <option value="2">Knock that Shit Off (2 seconds)</option>
  <option value="3">Bitch Please! (3 seconds)</option>
  <option value="4">Oh No, You Didn't! (4 seconds)</option>
  <option value="5">Bring the Pain! (5 seconds)</option>
</select>

<button id="activate" onclick="activate()">
  Fuck that Dawg!
</button>

<p id="status">Lets do this!</p>

</main>

<script>
let remaining = 0;
let timer = null;

async function activate() {
  const button = document.getElementById("activate");
  const status = document.getElementById("status");
  const seconds = Number(
    document.getElementById("duration").value
  );

  button.disabled = true;
  status.textContent = "This is what we trained for!!!";
  button.textContent = "Dog suffering in progress...";

  try {
    const response = await fetch(
      "/activate?seconds=" + seconds,
      {method: "POST"}
    );

    if (!response.ok) throw new Error("Request failed");

    remaining = seconds;
    status.textContent = "Suffering Remaining: " + remaining + " seconds ...";

    clearInterval(timer);
    timer = setInterval(() => {
      remaining--;
      if (remaining <= 0) {
        clearInterval(timer);
        status.textContent = "Lets do this Again!";
        button.textContent = "Fuck that Dawg!";
        button.disabled = false;
      } else {
        status.textContent =
          "Suffering Remaining: " + remaining + " seconds ...";
      }
    }, 1000);

  } catch (error) {
    status.textContent = "Unable to activate";
    button.textContent = "Fuck that Dawg!";
    button.disabled = false;
  }
}
</script>
</body>
</html>
)HTML";
