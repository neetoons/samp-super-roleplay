require("dotenv").config();
require("./database/connection.js");
const { app } = require("./app.js");
const { logger } = require("./utils/logger.js");

app.listen(process.env.PORT, () => {
  const a = logger.serverLogger();
  const text = `Server ready in http://localhost:${process.env.PORT}`;
  console.log(text);
  a.info(text);
});
