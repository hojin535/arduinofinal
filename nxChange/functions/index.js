const functions = require("firebase-functions");
const admin = require("firebase-admin");
const axios = require("axios");
admin.initializeApp();
const database = admin.database();

exports.predictWeather = functions.database
    .ref("/location/nx")
    .onUpdate(async (change, context) => {
      try {
        const curr = new Date();
        const utc = curr.getTime() + curr.getTimezoneOffset() * 60 * 1000;
        const kr = new Date(utc + 9 * 60 * 60 * 1000);
        const year = kr.getUTCFullYear();
        const month = kr.getUTCMonth() + 1;
        const date = kr.getUTCDate();
        const hours = kr.getUTCHours();
        const minutes = kr.getUTCMinutes();
        const basemonth = `${month < 10 ? "0" + month : month}`;
        const basedate = `${date < 10 ? "0" + date : date}`;
        const basehours = `${hours < 10 ? "0" + hours : hours}`;
        const basemin = `${minutes < 10 ? "0" + minutes : minutes}`;
        const nx = change.after.val();
        const nyRef = database.ref("/location/ny");
        const nySnapshot = await nyRef.once("value");
        const nyValue = nySnapshot.val();
        // 초단기 실황
        const response = await axios.get(
            `http://apis.data.go.kr/1360000/VilageFcstInfoService_2.0/getUltraSrtNcst?serviceKey=XruUJ8qt0zIFd6SOXRSTxUTjQf5FYoWg0EvN4SDuwQKepEtek%2Fj3L4RnqJ2ntfHA1fweotfs4brzI4hTOvL6CA%3D%3D&numOfRows=100&pageNo=1&base_date=${year}${basemonth}${basedate}&base_time=${basehours}${basemin}&nx=${nx}&ny=${nyValue}&dataType=JSON`,
        );

        // 초단기 예측
        const response2 = await axios.get(
            `http://apis.data.go.kr/1360000/VilageFcstInfoService_2.0/getUltraSrtFcst?serviceKey=XruUJ8qt0zIFd6SOXRSTxUTjQf5FYoWg0EvN4SDuwQKepEtek%2Fj3L4RnqJ2ntfHA1fweotfs4brzI4hTOvL6CA%3D%3D&numOfRows=100&pageNo=1&base_date=${year}${basemonth}${basedate}&base_time=${basehours}${basemin}&nx=${nx}&ny=${nyValue}&dataType=JSON`,
        );

        const forecastData = response2.data.response.body.items.item;
        const temp = response.data.response.body.items.item[3].obsrValue;
        const rain = response.data.response.body.items.item[0].obsrValue;
        const wet = response.data.response.body.items.item[1].obsrValue;
        // 초단기예측에서 sky값
        const preSky = response2.data.response.body.items.item[7].fcstValue;

        await database.ref("/temp").set(temp);
        await database.ref("/rain").set(rain);
        await database.ref("/wet").set(wet);
        await database.ref("/preSky").set(preSky);
        await database.ref("/forecastData").set(forecastData);
        console.log("Data saved to database.");
      } catch (error) {
        console.error(error);
      }
    });
