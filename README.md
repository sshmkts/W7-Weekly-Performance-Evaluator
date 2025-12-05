## Weekly Performance Evaluator – Week 7

This program is a console-based “Weekly Performance Evaluator” for a soccer player.  
It continues my Week 2–6 project and now uses **structs, arrays, enums, functions and a menu system** to analyze a full training week.

### What the app does

- Asks for the player’s **name and age** and stores everything in a `PlayerWeek` struct.
- Lets the player enter up to **5 training sessions** (hours per session are stored in an array).
- Calculates **total training time**, **average hours per session**, and **average sleep per night**.
- Uses an enum `PlayerLevel` to classify the week as  
  **Amateur**, **Semi-Pro**, or **Pro** based on training volume and sleep quality.
- Builds a **readiness score** and short **coach-style advice** for the player.

### Menu options

After the data is entered, the user can choose from a simple menu:

1. **Evaluate Level** –  
   Shows level (Amateur / Semi-Pro / Pro), totals, averages, readiness score, advice,  
   and a clean table with all sessions. Also saves a `LEVEL` summary to `report.txt`.

2. **Training Plan** –  
   Generates a one-session plan with **technical minutes + conditioning minutes**.  
   The plan depends on the level (Beginner, Semi-Pro, Pro) and can shift more time  
   to technical work if sleep is low. Saves a `TRAINING PLAN` section to `report.txt`.

3. **Recovery** –  
   Estimates **fatigue level** (Low / Moderate / High), suggests how many **rest days**  
   to take, and gives a short recovery tip (sleep goal, foam rolling, stretching).  
   Saves a `RECOVERY` section to `report.txt`.

0. **Quit** – exits the program.



