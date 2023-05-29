const functions = require("firebase-functions");
const admin = require("firebase-admin");
const axios = require("axios");

admin.initializeApp();

const database = admin.database();

exports.predictedApi = functions
    .region("asia-northeast3")
    .pubsub
    .schedule("25 * * * *")
    .timeZone("Asia/Seoul")
    .onRun(
        async (context) => {
          try {
            const response = await axios.get(
                `http://apis.data.go.kr/B552584/ArpltnInforInqireSvc/getMsrstnAcctoRltmMesureDnsty?stationName=상대동(진주)&numOfRows=1&dataTerm=DAILY&pageNo=1&ver=1.0&numOfRows=100&returnType=json&serviceKey=XruUJ8qt0zIFd6SOXRSTxUTjQf5FYoWg0EvN4SDuwQKepEtek%2Fj3L4RnqJ2ntfHA1fweotfs4brzI4hTOvL6CA%3D%3D` );
            const pm10Value =response.data.response.body.items[0].pm10Value;
            const pm25Value =response.data.response.body.items[0].pm25Value;
            const khaiGrade =response.data.response.body.items[0].khaiGrade;
            await database.ref("/pm10").set(pm10Value);
            await database.ref("/pm25").set(pm25Value);
            await database.ref("/khaiGrade").set(khaiGrade);

            console.log("Data saved to database.");
          } catch (error) {
            console.error(error);
          }
        });
