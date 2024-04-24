class userController {
  /**
   * @params {req} - Request of Express.
   * @params {res} - Response for User.
   * @function getAllInfoUser
   */
  async getAllInfoUser(req, res) {
    return res.json({ data: "Hola" });
  }
}

module.exports = new userController();
