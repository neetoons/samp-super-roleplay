const { Sequelize } = require("sequelize");
const { logger, databaseLoger, errorlog } = require("../utils/logger");
const sequelize = new Sequelize(
  process.env.DB_NAME,
  process.env.DB_USER,
  process.env.DB_PASS,
  {
    host: process.env.DB_HOST,
    port: process.env.DB_PORT,
    dialect: "mysql",
    logging: false,
  }
);

sequelize
  .authenticate()
  .then(() => {
    console.log("Database Connected");
    databaseLoger.info("Database Connected");
  })
  .catch((e) => console.error(e));

module.exports = { sequelize };
