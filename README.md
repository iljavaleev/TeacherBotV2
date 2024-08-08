## Bot for tutors.

### Second version. I wanted to consolidate the theoretical knowledge I gained from reading first part of Anthony Williams' wonderful book "C++ Concurrency in Action". The use of concurrency here is largely redundant, but it is for educational purposes.

### Key features
1. The bot “belongs” to the administrator - the teacher, who can give access to the bot to other teachers.
2. Each teacher can have several students and each student can have several teachers.
3. Each teacher, including the administrator, can create classes for a specific student, which will take place on a selected day of the week. When created, a message will be sent to the student.
4. The user can go to the bot and make a request to register as a teacher (can be approved by the admin) or a student (can be approved by the selected teacher).  
5. The Teacher/Student can look at the calendar on which days he has classes and see detailed information. 
6. A student may request to cancel a class. A message with the request will be sent to the teacher. 


## Run locally
1. Go to [ngrok.com](https://ngrok.com/), register if you don’t have an account yet.
2. After logging in, go to the “Your Authtoken” tab and copy the token from there to the .env file in NGROK_AUTHTOKEN.
3. Next, go to “Cloud Edge / Domains” and click “+ New Domain”.
4. Create your permanent domain and add it to the .env file in two places:
   * in NGROK_URL with "myurl.ngrok-free.app"-like format
   * in BASE_WEBHOOK_URL with "https://myurl.ngrok-free.app"-like format.
6. We fill out the rest of the .env file according to the instructions from .env.example.
7. We launch the project from the root directory (/backend) with the command.
  
   ```bash
   docker compose --env-file .env -f cppinfra/docker-compose.local.yml up -d
   ```

### !!! To run on the server in cppinfra/ swqg_nginx.conf you need to change the server url (required hostname, not ip). 