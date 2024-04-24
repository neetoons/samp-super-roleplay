const { Router } = require("express");
const indexRouter = Router();

const { userRoute } = require("./user");

indexRouter.use("/user", userRoute);
