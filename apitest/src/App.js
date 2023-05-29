import React, { useState, useCallback } from 'react';
import axios from 'axios';

const App = () => {
  const [data, setData] = useState(null);
const API_KEY = "0eba42189831bf14a855ecba63938f1a";
const [city, setCity] = useState("Seoul");
  //onClick 함수
  const onClick = async () => {
    try {
      const response = await axios.get(
        `https://api.openweathermap.org/data/2.5/weather?&lat=35.1799817&lon=128.1076213&lang=kr&appid=${API_KEY}`
      );
      setData(response.data);
    } catch (e) {
      console.log(e);
    }
  };
  const onConsole=()=>{
    console.log(data);
  }
return(
  <div>
    <div>
      <button onClick={onClick}>불러오기</button>
      <button onClick={onConsole}>보여주기</button>
    </div>

  </div>
  )
};

export default App;
