module.exports = (sequelize, DataTypes) => {
    return sequelize.define("pworks_points",
        {
            points: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            last_prize: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};