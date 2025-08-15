import axios from "axios";
import { TikTokLiveConnection, WebcastEvent } from "tiktok-live-connector";

const tiktokUsername = "bims_stagram";
const espUrl = "http://192.168.1.15"; // Ganti dengan IP NodeMCU kamu

const connection = new TikTokLiveConnection(tiktokUsername);

connection
  .connect()
  .then((state) => {
    console.info(`✅ Connected to roomId ${state.roomId}`);
  })
  .catch((err) => {
    console.error("❌ Failed to connect:", err);
  });

// Fungsi untuk delay (async sleep)
function delay(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

// Fungsi utama untuk trigger servo
async function handleGift(giftId, sender) {
  console.log(`🎁 ${sender} sent gift ID ${giftId}`);

  try {
    // Kirim ke ESP untuk servo gerak kanan
    await axios.get(`${espUrl}/servo?pos=r`);
    console.log("➡️ Servo moved right");

    // Tunggu 1 detik
    await delay(1000);

    // Kirim ke ESP untuk servo gerak kiri
    await axios.get(`${espUrl}/servo?pos=l`);
    console.log("⬅️ Servo moved left");
  } catch (err) {
    console.error("❌ Error sending to ESP:", err.message);
  }
}

// Event listener untuk gift TikTok
connection.on(WebcastEvent.GIFT, async (data) => {
  const { giftId, user, repeatEnd } = data;

  // Tunggu sampai gift selesai di-spam
  if (repeatEnd) {
    await handleGift(giftId, user.uniqueId);
  }
});
