const { Router } = require("express");
const indexRouter = Router();

/* Modules */
const { userRoute } = require("./user");

indexRouter.use("/user", userRoute);

module.exports = { indexRouter };
