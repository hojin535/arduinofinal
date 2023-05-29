import { Divider, Paper, Typography } from "@mui/material";
import { Box } from "@mui/system";
import React, { useState } from "react";
import "./App.css";
function App() {
  const [temp, setTemp] = useState("18.6");
  return (
    <div>
      <Paper elevation={5} sx={{ margin: "0.5rem" }}>
        <Box sx={{ paddingTop: "0.5rem" }}>
          <Typography variant="h3" sx={{ textAlign: "center" }}>
            <p style={{ fontWeight: "bold" }}>날씨</p>
          </Typography>
        </Box>
        <Divider sx={{ margin: "0.5rem" }} />
        <Typography variant="h4">{temp}°C</Typography>
      </Paper>
    </div>
  );
}
// 한국저작권위원회 의 "교육_아이콘_날씨_맑음_007" 은 CC BY 라이선스로 제공됩니다.
// 한국저작권위원회 의 "교육_아이콘_날씨_비_010" 은 CC BY 라이선스로 제공됩니다.
// 한국저작권위원회 의 "교육_아이콘_날씨_눈비_004" 은 CC BY 라이선스로 제공됩니다.
// 한국저작권위원회 의 "교육_아이콘_날씨_눈_003" 은 CC BY 라이선스로 제공됩니다.
// 한국저작권위원회 의 "교육_아이콘_날씨_소나기_011" 은 CC BY 라이선스로 제공됩니다.
export default App;
