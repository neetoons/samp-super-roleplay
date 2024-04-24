const winston = require("winston");

const logger = {
  serverLogger: () => {
    return winston.createLogger({
      level: "info",
      format: winston.format.combine(
        winston.format.timestamp(),
        winston.format.printf((info) => {
          return `${info.timestamp} - ${info.level}: ${info.message}`;
        })
      ),
      transports: [
        new winston.transports.File({
          filename: "./logs/logfile.log",
          level: "info",
        }),
      ],
    });
  },
  databaseLogger: () => {
    return winston.createLogger({
      level: "info",
      format: winston.format.combine(
        winston.format.timestamp(),
        winston.format.printf((info) => {
          return `${info.timestamp} - ${info.level}: ${info.message}`;
        })
      ),
      transports: [
        new winston.transports.File({
          filename: "./logs/database.log",
          level: "info",
        }),
      ],
    });
  },
  errorLogger: () => {
    const errorLogger = winston.createLogger({
      level: "error",
      format: winston.format.combine(
        winston.format.timestamp(),
        winston.format.printf((error) => {
          return `${error.timestamp} - ${error.level}: ${error.message}`;
        })
      ),
      transports: [
        new winston.transports.Console(),
        new winston.transports.File({
          filename: "./logs/logfile.log",
          level: "info",
        }),
      ],
    });
  },
};

const databaseLoger = logger.databaseLogger();
const errorlog = logger.errorLogger();

module.exports = { logger, databaseLoger, errorlog };
