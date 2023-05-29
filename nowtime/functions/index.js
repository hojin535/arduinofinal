const functions = require("firebase-functions");
const admin = require("firebase-admin");
// const axios = require("axios");
// !!!!!!!!!!!!!!!!!!!!!!!!!! 안쓸거임


admin.initializeApp();

const database = admin.database();

exports.scheduledFunctionCrontab = functions
    .region("asia-northeast3").pubsub
    .schedule("0,10,20,30,40,50 * * * *")
    .timeZone("Asia/Seoul")
    .onRun(
        async (context) => {
          try {
            const curr = new Date();
            const utc =
            curr.getTime() +
            (curr.getTimezoneOffset() * 60 * 1000);
            const kr = new Date(utc+(9*60*60*1000));
            let hours=kr.getHours();
            const minutes = kr.getMinutes();
            if (hours<10) {
              hours ="0"+hours;
            }
            if (minutes<10) {
              hours ="0"+hours;
            }
            const time =String(hours)+String(minutes);
            await database.ref("/time").set(time);


            console.log("Data saved to database.");
          } catch (error) {
            console.error(error);
          }
        });
